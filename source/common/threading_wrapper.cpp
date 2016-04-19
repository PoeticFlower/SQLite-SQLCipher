#include "common.h"
#include "threading_wrapper.h"

#define STACK_ALIGN(func, ...) func(__VA_ARGS__)

Lock::Lock()
{
#ifdef _WIN32
  InitializeCriticalSection(&this->m_handle);
#else  //  POSIX
  pthread_mutex_init       (&this->m_handle, NULL);
#endif // _WIN32
}

Lock::~Lock()
{
#ifdef _WIN32
  DeleteCriticalSection(&this->m_handle);
#else  //  POSIX
  pthread_mutex_destroy(&this->m_handle);
#endif // _WIN32
}

void Lock::Acquire()
{
#ifdef _WIN32
  EnterCriticalSection(&this->m_handle);
#else  //  POSIX
  pthread_mutex_lock  (&this->m_handle);
#endif // _WIN32
}

void Lock::Release()
{
#ifdef _WIN32
  LeaveCriticalSection(&this->m_handle);
#else  //  POSIX
  pthread_mutex_unlock(&this->m_handle);
#endif // _WIN32
}

Event::Event()
{
#ifdef _WIN32
  this->m_handle = CreateEvent(NULL, FALSE, FALSE, NULL);
#else  //  POSIX
  m_counter = 0;
  if ( pthread_mutex_init(&m_mutex, NULL)
    || pthread_cond_init (&m_cond,  NULL))
  {
    fprintf(stderr,
      "Event Error: unable to initialize conditional variable\n");
  }
#endif // _WIN32
}

Event::~Event()
{
#ifdef _WIN32
  CloseHandle          (this->m_handle);
#else  //  POSIX
  pthread_cond_destroy (&m_cond);
  pthread_mutex_destroy(&m_mutex);
#endif // _WIN32
}

void Event::Wait()
{
#ifdef _WIN32
  WaitForSingleObject (this->m_handle, INFINITE);
#else  //  POSIX
  pthread_mutex_lock  (&m_mutex);
  while (!m_counter)
    pthread_cond_wait (&m_cond, &m_mutex);
  m_counter--;
  pthread_mutex_unlock(&m_mutex);
#endif // _WIN32
}

bool Event::TimedWait(uint32_t milliseconds)
{
  /* returns true if the wait timed out */
#ifdef _WIN32
  return WaitForSingleObject(this->m_handle, milliseconds) == WAIT_TIMEOUT;
#else  //  POSIX
  bool timed_out = false;
  pthread_mutex_lock(&m_mutex);
  if (!m_counter)
  {
    struct timeval  tv;
    struct timespec ts;
    gettimeofday(&tv, NULL);
    // convert current time from (sec, usec) to (sec, nsec)
    ts.tv_sec   = tv.tv_sec;
    ts.tv_nsec  = tv.tv_usec * 1000;
    ts.tv_nsec += 1000 * 1000 * (milliseconds % 1000); // add ms to tv_nsec
    ts.tv_sec  += ts.tv_nsec / (1000 * 1000 * 1000);   // overflow tv_nsec
    ts.tv_nsec %= (1000 * 1000 * 1000);                // clamp tv_nsec
    ts.tv_sec  += milliseconds / 1000;                 // add seconds
    timed_out   = pthread_cond_timedwait(&m_cond, &m_mutex, &ts) == ETIMEDOUT;
  }
  if (m_counter > 0)
  {
    m_counter--;
    timed_out = false;
  }
  pthread_mutex_unlock(&m_mutex);
  return timed_out;
#endif // _WIN32
}

void Event::Trigger()
{
#ifdef _WIN32
  SetEvent            (this->m_handle);
#else  //  POSIX
  pthread_mutex_lock  (&m_mutex);
  if (m_counter < UINT32_MAX)
    m_counter++;
  /* Signal a single blocking thread */
  pthread_cond_signal (&m_cond);
  pthread_mutex_unlock(&m_mutex);
#endif // _WIN32
}
