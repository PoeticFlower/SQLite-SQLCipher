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
