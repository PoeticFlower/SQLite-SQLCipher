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

#if defined(_WIN32)
static void* WinGetProcAddress(const char* name)
{
  static bool    initialized = false;
  static HMODULE handle      = NULL;
  if (!handle && !initialized)
  {
    Lock       lock;
    ScopedLock scoped_lock(lock);
    if (!initialized)
    {
      handle = GetModuleHandleA("OpenCL.dll");
      if (!handle)
      {
        const char* path = "OpenCL.dll";
        handle = LoadLibraryA(path);
        if (!handle)
        {
          OPENCL_LOG_ERROR("Failed to load OpenCL runtime\n");
        }
      }
      initialized = true;
    }
  }
  if (!handle)
    return NULL;
  return (void*)GetProcAddress(handle, name);
}

#define OPENCL_GET_PROC_ADDRESS(name)                                       \
  if (NULL == (func_##name = (FUNC_##name)WinGetProcAddress(#name)))        \
  {                                                                         \
    OPENCL_LOG_ERROR("Failed to load OpenCL API: %s\n", #name);             \
    return false;                                                           \
  }
#endif // _WIN32

#if defined(__linux__)
static void* UnixGetProcAddress(const char* name)
{
  static bool    initialized = false;
  static void   *handle      = NULL;
  if (!handle && !initialized)
  {
    Lock       lock;
    ScopedLock scoped_lock(lock);
    if (!initialized)
    {
      const char* path = "libOpenCL.so";
      handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
      if (!handle)
      {
        path = "libOpenCL.so.1";
        handle = dlopen(path, RTLD_LAZY | RTLD_GLOBAL);
        if (!handle)
        {
          OPENCL_LOG_ERROR("Failed to load OpenCL runtime\n");
        }
      }
      initialized = true;
    }
  }
  if (!handle)
    return NULL;
  return dlsym(handle, name);
}

#define OPENCL_GET_PROC_ADDRESS(name)                                       \
  if (NULL == (func_##name = (FUNC_##name)UnixGetProcAddress(#name)))       \
  {                                                                         \
    OPENCL_LOG_ERROR("Failed to load OpenCL API: %s\n", #name);             \
    return false;                                                           \
  }
#endif // __linux__

#ifndef OPENCL_GET_PROC_ADDRESS
#ifdef __GNUC__
#warning("OpenCL dynamic library loader: check configuration")
#else
#pragma message("WARNING: OpenCL dynamic library loader: check configuration")
#endif
#define OPENCL_GET_PROC_ADDRESS(name) NULL
#endif // OPENCL_GET_PROC_ADDRESS

static const char* OpenCLGetErrorString(cl_int ocl_err_code)
{
  static const char* strings[] =
  {
      "CL_SUCCESS"                                             /*   0 */
    , "CL_DEVICE_NOT_FOUND"                                    /*  -1 */
    , "CL_DEVICE_NOT_AVAILABLE"                                /*  -2 */
    , "CL_COMPILER_NOT_AVAILABLE"                              /*  -3 */
    , "CL_MEM_OBJECT_ALLOCATION_FAILURE"                       /*  -4 */
    , "CL_OUT_OF_RESOURCES"                                    /*  -5 */
    , "CL_OUT_OF_HOST_MEMORY"                                  /*  -6 */
    , "CL_PROFILING_INFO_NOT_AVAILABLE"                        /*  -7 */
    , "CL_MEM_COPY_OVERLAP"                                    /*  -8 */
    , "CL_IMAGE_FORMAT_MISMATCH"                               /*  -9 */
    , "CL_IMAGE_FORMAT_NOT_SUPPORTED"                          /* -10 */
    , "CL_BUILD_PROGRAM_FAILURE"                               /* -11 */
    , "CL_MAP_FAILURE"                                         /* -12 */
    , "CL_MISALIGNED_SUB_BUFFER_OFFSET"                        /* -13 */
    , "CL_EXEC_STATUS_ERROR_FOR_EVENTS_IN_WAIT_LIST"           /* -14 */
    , "CL_COMPILE_PROGRAM_FAILURE"                             /* -15 */
    , "CL_LINKER_NOT_AVAILABLE"                                /* -16 */
    , "CL_LINK_PROGRAM_FAILURE"                                /* -17 */
    , "CL_DEVICE_PARTITION_FAILED"                             /* -18 */
    , "CL_KERNEL_ARG_INFO_NOT_AVAILABLE"                       /* -19 */
    , "UNDEFINED_ERROR_CODE"                                   /* -20 */
    , "UNDEFINED_ERROR_CODE"                                   /* -21 */
    , "UNDEFINED_ERROR_CODE"                                   /* -22 */
    , "UNDEFINED_ERROR_CODE"                                   /* -23 */
    , "UNDEFINED_ERROR_CODE"                                   /* -24 */
    , "UNDEFINED_ERROR_CODE"                                   /* -25 */
    , "UNDEFINED_ERROR_CODE"                                   /* -26 */
    , "UNDEFINED_ERROR_CODE"                                   /* -27 */
    , "UNDEFINED_ERROR_CODE"                                   /* -28 */
    , "UNDEFINED_ERROR_CODE"                                   /* -29 */
    , "CL_INVALID_VALUE"                                       /* -30 */
    , "CL_INVALID_DEVICE_TYPE"                                 /* -31 */
    , "CL_INVALID_PLATFORM"                                    /* -32 */
    , "CL_INVALID_DEVICE"                                      /* -33 */
    , "CL_INVALID_CONTEXT"                                     /* -34 */
    , "CL_INVALID_QUEUE_PROPERTIES"                            /* -35 */
    , "CL_INVALID_COMMAND_QUEUE"                               /* -36 */
    , "CL_INVALID_HOST_PTR"                                    /* -37 */
    , "CL_INVALID_MEM_OBJECT"                                  /* -38 */
    , "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR"                     /* -39 */
    , "CL_INVALID_IMAGE_SIZE"                                  /* -40 */
    , "CL_INVALID_SAMPLER"                                     /* -41 */
    , "CL_INVALID_BINARY"                                      /* -42 */
    , "CL_INVALID_BUILD_OPTIONS"                               /* -43 */
    , "CL_INVALID_PROGRAM"                                     /* -44 */
    , "CL_INVALID_PROGRAM_EXECUTABLE"                          /* -45 */
    , "CL_INVALID_KERNEL_NAME"                                 /* -46 */
    , "CL_INVALID_KERNEL_DEFINITION"                           /* -47 */
    , "CL_INVALID_KERNEL"                                      /* -48 */
    , "CL_INVALID_ARG_INDEX"                                   /* -49 */
    , "CL_INVALID_ARG_VALUE"                                   /* -50 */
    , "CL_INVALID_ARG_SIZE"                                    /* -51 */
    , "CL_INVALID_KERNEL_ARGS"                                 /* -52 */
    , "CL_INVALID_WORK_DIMENSION"                              /* -53 */
    , "CL_INVALID_WORK_GROUP_SIZE"                             /* -54 */
    , "CL_INVALID_WORK_ITEM_SIZE"                              /* -55 */
    , "CL_INVALID_GLOBAL_OFFSET"                               /* -56 */
    , "CL_INVALID_EVENT_WAIT_LIST"                             /* -57 */
    , "CL_INVALID_EVENT"                                       /* -58 */
    , "CL_INVALID_OPERATION"                                   /* -59 */
    , "CL_INVALID_GL_OBJECT"                                   /* -60 */
    , "CL_INVALID_BUFFER_SIZE"                                 /* -61 */
    , "CL_INVALID_MIP_LEVEL"                                   /* -62 */
    , "CL_INVALID_GLOBAL_WORK_SIZE"                            /* -63 */
    , "CL_INVALID_PROPERTY"                                    /* -64 */
    , "CL_INVALID_IMAGE_DESCRIPTOR"                            /* -65 */
    , "CL_INVALID_COMPILER_OPTIONS"                            /* -66 */
    , "CL_INVALID_LINKER_OPTIONS"                              /* -67 */
    , "CL_INVALID_DEVICE_PARTITION_COUNT"                      /* -68 */
  };

  if ( ocl_err_code > 0 || ocl_err_code < -68 )
    return "UNKNOWN_ERROR_CODE";
  return strings[-ocl_err_code];
}
