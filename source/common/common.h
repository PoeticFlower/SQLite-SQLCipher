#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cstring>

#if defined(_WIN32)
#ifndef   GUI_APPLICATION_USING_MFC
#include <windows.h>
#endif // GUI_APPLICATION_USING_MFC
#else                             //  POSIX
#include <dlfcn.h>
#include <pthread.h>
#include <errno.h>
#include <sys/time.h>
#endif                            // #if defined(_WIN32)
