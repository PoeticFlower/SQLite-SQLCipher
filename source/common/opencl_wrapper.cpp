#include "common.h"
#include "opencl_wrapper.h"
#include "threading_wrapper.h"
#include <CL/opencl.h>

/*****************************************************************************
 * OpenCL API Wrapper - Start
 *****************************************************************************/

/*****************************************************************************
 * OpenCL API Wrapper - End
 *****************************************************************************/

#define OCL_FUNC_LOAD_INIT    0
#define OCL_FUNC_LOAD_SUCCESS 1
#define OCL_FUNC_LOAD_FAILED  2
#define OCL_MAX_NUM_DEVICES   128

struct OpenCLExecuteStruct
{
public:
  OpenCLExecuteStruct()
    : m_num_devices(0)
    , m_num_command_queues(0)
    , m_context(NULL)
  {}
public:
  cl_uint           m_num_devices;
  cl_uint           m_num_command_queues;
  cl_device_id      m_device_list[OCL_MAX_NUM_DEVICES];
  cl_context        m_context;
  cl_command_queue *m_command_queue_list;
};

unsigned int OpenCLWrapper::m_opencl_func_loading_status = OCL_FUNC_LOAD_INIT;

#define OPENCL_LOG_ERROR(...)                                               \
  fprintf(stderr, __VA_ARGS__)

#define OPENCL_SAFE_CALL0(oclfunc, action)                                  \
  do                                                                        \
  {                                                                         \
    oclfunc;                                                                \
    if (CL_SUCCESS != ocl_err_code)                                         \
    {                                                                       \
      OPENCL_LOG_ERROR(                                                     \
        "Failed to call OpenCL API %s: (CODE-%d)%s, Line %u.\n",            \
        #oclfunc,                                                           \
        ocl_err_code,                                                       \
        OpenCLGetErrorString(ocl_err_code),                                 \
        __LINE__                                                            \
        );                                                                  \
      action;                                                               \
    }                                                                       \
  } while (0)

#if defined(__APPLE__)
static void* AppleGetProcAddress(const char* name)
{
  static bool    initialized = false;
  static void   *handle      = NULL;
  if (!handle && !initialized)
  {
    Lock       lock;
    ScopedLock scoped_lock(lock);
    if (!initialized)
    {
      const char* path =
        "/System/Library/Frameworks/OpenCL.framework/Versions/Current/OpenCL";
      handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
      if (!handle)
      {
        OPENCL_LOG_ERROR("Failed to load OpenCL runtime\n");
      }
      initialized = true;
    }
  }
  if (!handle)
    return NULL;
  return dlsym(handle, name);
}

#define OPENCL_GET_PROC_ADDRESS(name)                                       \
  if (NULL == (func_##name = (FUNC_##name)AppleGetProcAddress(#name)))      \
  {                                                                         \
    OPENCL_LOG_ERROR("Failed to load OpenCL API: %s\n", #name);             \
    return false;                                                           \
  }
#endif // __APPLE__

#ifndef OPENCL_GET_PROC_ADDRESS
#ifdef __GNUC__
#warning("OpenCL dynamic library loader: check configuration")
#else
#pragma message("WARNING: OpenCL dynamic library loader: check configuration")
#endif
#define OPENCL_GET_PROC_ADDRESS(name) NULL
#endif // OPENCL_GET_PROC_ADDRESS
