#include "common.h"
#include "opencl_wrapper.h"
#include "threading_wrapper.h"
#include <CL/opencl.h>

/*****************************************************************************
 * OpenCL API Wrapper - Start
 *****************************************************************************/

typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetPlatformIDs                        )(cl_uint , cl_platform_id * , cl_uint * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetPlatformInfo                       )(cl_platform_id , cl_platform_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetDeviceIDs                          )(cl_platform_id , cl_device_type , cl_uint , cl_device_id * , cl_uint * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetDeviceInfo                         )(cl_device_id , cl_device_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_context       (CL_API_CALL *FUNC_clCreateContext                         )(const cl_context_properties * , cl_uint , const cl_device_id * , void (CL_CALLBACK * )(const char *, const void *, size_t, void *), void * , cl_int * );
typedef CL_API_ENTRY cl_context       (CL_API_CALL *FUNC_clCreateContextFromType                 )(const cl_context_properties * , cl_device_type , void (CL_CALLBACK * )(const char *, const void *, size_t, void *), void * , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainContext                         )(cl_context );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseContext                        )(cl_context );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetContextInfo                        )(cl_context , cl_context_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_command_queue (CL_API_CALL *FUNC_clCreateCommandQueue                    )(cl_context , cl_device_id , cl_command_queue_properties , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainCommandQueue                    )(cl_command_queue );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseCommandQueue                   )(cl_command_queue );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetCommandQueueInfo                   )(cl_command_queue , cl_command_queue_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clSetCommandQueueProperty               )(cl_command_queue , cl_command_queue_properties , cl_bool , cl_command_queue_properties * );
typedef CL_API_ENTRY cl_mem           (CL_API_CALL *FUNC_clCreateBuffer                          )(cl_context , cl_mem_flags , size_t , void * , cl_int * );
typedef CL_API_ENTRY cl_mem           (CL_API_CALL *FUNC_clCreateImage2D                         )(cl_context , cl_mem_flags , const cl_image_format * , size_t , size_t , size_t , void * , cl_int * );
typedef CL_API_ENTRY cl_mem           (CL_API_CALL *FUNC_clCreateImage3D                         )(cl_context , cl_mem_flags , const cl_image_format * , size_t , size_t , size_t , size_t , size_t , void * , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainMemObject                       )(cl_mem );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseMemObject                      )(cl_mem );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetSupportedImageFormats              )(cl_context , cl_mem_flags , cl_mem_object_type , cl_uint , cl_image_format * , cl_uint * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetMemObjectInfo                      )(cl_mem , cl_mem_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetImageInfo                          )(cl_mem , cl_image_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_sampler       (CL_API_CALL *FUNC_clCreateSampler                         )(cl_context , cl_bool , cl_addressing_mode , cl_filter_mode , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainSampler                         )(cl_sampler );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseSampler                        )(cl_sampler );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetSamplerInfo                        )(cl_sampler , cl_sampler_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_program       (CL_API_CALL *FUNC_clCreateProgramWithSource               )(cl_context , cl_uint , const char ** , const size_t * , cl_int * );
typedef CL_API_ENTRY cl_program       (CL_API_CALL *FUNC_clCreateProgramWithBinary               )(cl_context , cl_uint , const cl_device_id * , const size_t * , const unsigned char ** , cl_int * , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainProgram                         )(cl_program );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseProgram                        )(cl_program );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clBuildProgram                          )(cl_program , cl_uint , const cl_device_id * , const char * , void (CL_CALLBACK * )(cl_program , void * ), void * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clUnloadCompiler                        )(void);
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetProgramInfo                        )(cl_program , cl_program_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetProgramBuildInfo                   )(cl_program , cl_device_id , cl_program_build_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_kernel        (CL_API_CALL *FUNC_clCreateKernel                          )(cl_program , const char * , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clCreateKernelsInProgram                )(cl_program , cl_uint , cl_kernel * , cl_uint * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainKernel                          )(cl_kernel );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseKernel                         )(cl_kernel );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clSetKernelArg                          )(cl_kernel , cl_uint , size_t , const void * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetKernelInfo                         )(cl_kernel , cl_kernel_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetKernelWorkGroupInfo                )(cl_kernel , cl_device_id , cl_kernel_work_group_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clWaitForEvents                         )(cl_uint , const cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetEventInfo                          )(cl_event , cl_event_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clRetainEvent                           )(cl_event );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clReleaseEvent                          )(cl_event );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clGetEventProfilingInfo                 )(cl_event , cl_profiling_info , size_t , void * , size_t * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clFlush                                 )(cl_command_queue );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clFinish                                )(cl_command_queue );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueReadBuffer                     )(cl_command_queue , cl_mem , cl_bool , size_t , size_t , void * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueWriteBuffer                    )(cl_command_queue , cl_mem , cl_bool , size_t , size_t , const void * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueCopyBuffer                     )(cl_command_queue , cl_mem , cl_mem , size_t , size_t , size_t , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueReadImage                      )(cl_command_queue , cl_mem , cl_bool , const size_t * , const size_t * , size_t , size_t , void * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueWriteImage                     )(cl_command_queue , cl_mem , cl_bool , const size_t * , const size_t * , size_t , size_t , const void * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueCopyImage                      )(cl_command_queue , cl_mem , cl_mem , const size_t * , const size_t * , const size_t * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueCopyImageToBuffer              )(cl_command_queue , cl_mem , cl_mem , const size_t * , const size_t * , size_t , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueCopyBufferToImage              )(cl_command_queue , cl_mem , cl_mem , size_t , const size_t * , const size_t * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY void *           (CL_API_CALL *FUNC_clEnqueueMapBuffer                      )(cl_command_queue , cl_mem , cl_bool , cl_map_flags , size_t , size_t , cl_uint , const cl_event * , cl_event * , cl_int * );
typedef CL_API_ENTRY void *           (CL_API_CALL *FUNC_clEnqueueMapImage                       )(cl_command_queue , cl_mem , cl_bool , cl_map_flags , const size_t * , const size_t * , size_t * , size_t * , cl_uint , const cl_event * , cl_event * , cl_int * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueUnmapMemObject                 )(cl_command_queue , cl_mem , void * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueNDRangeKernel                  )(cl_command_queue , cl_kernel , cl_uint , const size_t * , const size_t * , const size_t * , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueTask                           )(cl_command_queue , cl_kernel , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueNativeKernel                   )(cl_command_queue , void (*user_func)(void *), void * , size_t , cl_uint , const cl_mem * , const void ** , cl_uint , const cl_event * , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueMarker                         )(cl_command_queue , cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueWaitForEvents                  )(cl_command_queue , cl_uint , const cl_event * );
typedef CL_API_ENTRY cl_int           (CL_API_CALL *FUNC_clEnqueueBarrier                        )(cl_command_queue );
typedef CL_API_ENTRY void *           (CL_API_CALL *FUNC_clGetExtensionFunctionAddress           )(const char * );

static FUNC_clGetPlatformIDs                         func_clGetPlatformIDs                         = NULL;
static FUNC_clGetPlatformInfo                        func_clGetPlatformInfo                        = NULL;
static FUNC_clGetDeviceIDs                           func_clGetDeviceIDs                           = NULL;
static FUNC_clGetDeviceInfo                          func_clGetDeviceInfo                          = NULL;
static FUNC_clCreateContext                          func_clCreateContext                          = NULL;
static FUNC_clCreateContextFromType                  func_clCreateContextFromType                  = NULL;
static FUNC_clRetainContext                          func_clRetainContext                          = NULL;
static FUNC_clReleaseContext                         func_clReleaseContext                         = NULL;
static FUNC_clGetContextInfo                         func_clGetContextInfo                         = NULL;
static FUNC_clCreateCommandQueue                     func_clCreateCommandQueue                     = NULL;
static FUNC_clRetainCommandQueue                     func_clRetainCommandQueue                     = NULL;
static FUNC_clReleaseCommandQueue                    func_clReleaseCommandQueue                    = NULL;
static FUNC_clGetCommandQueueInfo                    func_clGetCommandQueueInfo                    = NULL;
static FUNC_clSetCommandQueueProperty                func_clSetCommandQueueProperty                = NULL;
static FUNC_clCreateBuffer                           func_clCreateBuffer                           = NULL;
static FUNC_clCreateImage2D                          func_clCreateImage2D                          = NULL;
static FUNC_clCreateImage3D                          func_clCreateImage3D                          = NULL;
static FUNC_clRetainMemObject                        func_clRetainMemObject                        = NULL;
static FUNC_clReleaseMemObject                       func_clReleaseMemObject                       = NULL;
static FUNC_clGetSupportedImageFormats               func_clGetSupportedImageFormats               = NULL;
static FUNC_clGetMemObjectInfo                       func_clGetMemObjectInfo                       = NULL;
static FUNC_clGetImageInfo                           func_clGetImageInfo                           = NULL;
static FUNC_clCreateSampler                          func_clCreateSampler                          = NULL;
static FUNC_clRetainSampler                          func_clRetainSampler                          = NULL;
static FUNC_clReleaseSampler                         func_clReleaseSampler                         = NULL;
static FUNC_clGetSamplerInfo                         func_clGetSamplerInfo                         = NULL;
static FUNC_clCreateProgramWithSource                func_clCreateProgramWithSource                = NULL;
static FUNC_clCreateProgramWithBinary                func_clCreateProgramWithBinary                = NULL;
static FUNC_clRetainProgram                          func_clRetainProgram                          = NULL;
static FUNC_clReleaseProgram                         func_clReleaseProgram                         = NULL;
static FUNC_clBuildProgram                           func_clBuildProgram                           = NULL;
static FUNC_clUnloadCompiler                         func_clUnloadCompiler                         = NULL;
static FUNC_clGetProgramInfo                         func_clGetProgramInfo                         = NULL;
static FUNC_clGetProgramBuildInfo                    func_clGetProgramBuildInfo                    = NULL;
static FUNC_clCreateKernel                           func_clCreateKernel                           = NULL;
static FUNC_clCreateKernelsInProgram                 func_clCreateKernelsInProgram                 = NULL;
static FUNC_clRetainKernel                           func_clRetainKernel                           = NULL;
static FUNC_clReleaseKernel                          func_clReleaseKernel                          = NULL;
static FUNC_clSetKernelArg                           func_clSetKernelArg                           = NULL;
static FUNC_clGetKernelInfo                          func_clGetKernelInfo                          = NULL;
static FUNC_clGetKernelWorkGroupInfo                 func_clGetKernelWorkGroupInfo                 = NULL;
static FUNC_clWaitForEvents                          func_clWaitForEvents                          = NULL;
static FUNC_clGetEventInfo                           func_clGetEventInfo                           = NULL;
static FUNC_clRetainEvent                            func_clRetainEvent                            = NULL;
static FUNC_clReleaseEvent                           func_clReleaseEvent                           = NULL;
static FUNC_clGetEventProfilingInfo                  func_clGetEventProfilingInfo                  = NULL;
static FUNC_clFlush                                  func_clFlush                                  = NULL;
static FUNC_clFinish                                 func_clFinish                                 = NULL;
static FUNC_clEnqueueReadBuffer                      func_clEnqueueReadBuffer                      = NULL;
static FUNC_clEnqueueWriteBuffer                     func_clEnqueueWriteBuffer                     = NULL;
static FUNC_clEnqueueCopyBuffer                      func_clEnqueueCopyBuffer                      = NULL;
static FUNC_clEnqueueReadImage                       func_clEnqueueReadImage                       = NULL;
static FUNC_clEnqueueWriteImage                      func_clEnqueueWriteImage                      = NULL;
static FUNC_clEnqueueCopyImage                       func_clEnqueueCopyImage                       = NULL;
static FUNC_clEnqueueCopyImageToBuffer               func_clEnqueueCopyImageToBuffer               = NULL;
static FUNC_clEnqueueCopyBufferToImage               func_clEnqueueCopyBufferToImage               = NULL;
static FUNC_clEnqueueMapBuffer                       func_clEnqueueMapBuffer                       = NULL;
static FUNC_clEnqueueMapImage                        func_clEnqueueMapImage                        = NULL;
static FUNC_clEnqueueUnmapMemObject                  func_clEnqueueUnmapMemObject                  = NULL;
static FUNC_clEnqueueNDRangeKernel                   func_clEnqueueNDRangeKernel                   = NULL;
static FUNC_clEnqueueTask                            func_clEnqueueTask                            = NULL;
static FUNC_clEnqueueNativeKernel                    func_clEnqueueNativeKernel                    = NULL;
static FUNC_clEnqueueMarker                          func_clEnqueueMarker                          = NULL;
static FUNC_clEnqueueWaitForEvents                   func_clEnqueueWaitForEvents                   = NULL;
static FUNC_clEnqueueBarrier                         func_clEnqueueBarrier                         = NULL;
static FUNC_clGetExtensionFunctionAddress            func_clGetExtensionFunctionAddress            = NULL;

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

static bool OpenCLGetProcAddress()
{
  OPENCL_GET_PROC_ADDRESS(clGetPlatformIDs                        )
  OPENCL_GET_PROC_ADDRESS(clGetPlatformInfo                       )
  OPENCL_GET_PROC_ADDRESS(clGetDeviceIDs                          )
  OPENCL_GET_PROC_ADDRESS(clGetDeviceInfo                         )
  OPENCL_GET_PROC_ADDRESS(clCreateContext                         )
  OPENCL_GET_PROC_ADDRESS(clCreateContextFromType                 )
  OPENCL_GET_PROC_ADDRESS(clRetainContext                         )
  OPENCL_GET_PROC_ADDRESS(clReleaseContext                        )
  OPENCL_GET_PROC_ADDRESS(clGetContextInfo                        )
  OPENCL_GET_PROC_ADDRESS(clCreateCommandQueue                    )
  OPENCL_GET_PROC_ADDRESS(clRetainCommandQueue                    )
  OPENCL_GET_PROC_ADDRESS(clReleaseCommandQueue                   )
  OPENCL_GET_PROC_ADDRESS(clGetCommandQueueInfo                   )
  OPENCL_GET_PROC_ADDRESS(clSetCommandQueueProperty               )
  OPENCL_GET_PROC_ADDRESS(clCreateBuffer                          )
  OPENCL_GET_PROC_ADDRESS(clCreateImage2D                         )
  OPENCL_GET_PROC_ADDRESS(clCreateImage3D                         )
  OPENCL_GET_PROC_ADDRESS(clRetainMemObject                       )
  OPENCL_GET_PROC_ADDRESS(clReleaseMemObject                      )
  OPENCL_GET_PROC_ADDRESS(clGetSupportedImageFormats              )
  OPENCL_GET_PROC_ADDRESS(clGetMemObjectInfo                      )
  OPENCL_GET_PROC_ADDRESS(clGetImageInfo                          )
  OPENCL_GET_PROC_ADDRESS(clCreateSampler                         )
  OPENCL_GET_PROC_ADDRESS(clRetainSampler                         )
  OPENCL_GET_PROC_ADDRESS(clReleaseSampler                        )
  OPENCL_GET_PROC_ADDRESS(clGetSamplerInfo                        )
  OPENCL_GET_PROC_ADDRESS(clCreateProgramWithSource               )
  OPENCL_GET_PROC_ADDRESS(clCreateProgramWithBinary               )
  OPENCL_GET_PROC_ADDRESS(clRetainProgram                         )
  OPENCL_GET_PROC_ADDRESS(clReleaseProgram                        )
  OPENCL_GET_PROC_ADDRESS(clBuildProgram                          )
  OPENCL_GET_PROC_ADDRESS(clUnloadCompiler                        )
  OPENCL_GET_PROC_ADDRESS(clGetProgramInfo                        )
  OPENCL_GET_PROC_ADDRESS(clGetProgramBuildInfo                   )
  OPENCL_GET_PROC_ADDRESS(clCreateKernel                          )
  OPENCL_GET_PROC_ADDRESS(clCreateKernelsInProgram                )
  OPENCL_GET_PROC_ADDRESS(clRetainKernel                          )
  OPENCL_GET_PROC_ADDRESS(clReleaseKernel                         )
  OPENCL_GET_PROC_ADDRESS(clSetKernelArg                          )
  OPENCL_GET_PROC_ADDRESS(clGetKernelInfo                         )
  OPENCL_GET_PROC_ADDRESS(clGetKernelWorkGroupInfo                )
  OPENCL_GET_PROC_ADDRESS(clWaitForEvents                         )
  OPENCL_GET_PROC_ADDRESS(clGetEventInfo                          )
  OPENCL_GET_PROC_ADDRESS(clRetainEvent                           )
  OPENCL_GET_PROC_ADDRESS(clReleaseEvent                          )
  OPENCL_GET_PROC_ADDRESS(clGetEventProfilingInfo                 )
  OPENCL_GET_PROC_ADDRESS(clFlush                                 )
  OPENCL_GET_PROC_ADDRESS(clFinish                                )
  OPENCL_GET_PROC_ADDRESS(clEnqueueReadBuffer                     )
  OPENCL_GET_PROC_ADDRESS(clEnqueueWriteBuffer                    )
  OPENCL_GET_PROC_ADDRESS(clEnqueueCopyBuffer                     )
  OPENCL_GET_PROC_ADDRESS(clEnqueueReadImage                      )
  OPENCL_GET_PROC_ADDRESS(clEnqueueWriteImage                     )
  OPENCL_GET_PROC_ADDRESS(clEnqueueCopyImage                      )
  OPENCL_GET_PROC_ADDRESS(clEnqueueCopyImageToBuffer              )
  OPENCL_GET_PROC_ADDRESS(clEnqueueCopyBufferToImage              )
  OPENCL_GET_PROC_ADDRESS(clEnqueueMapBuffer                      )
  OPENCL_GET_PROC_ADDRESS(clEnqueueMapImage                       )
  OPENCL_GET_PROC_ADDRESS(clEnqueueUnmapMemObject                 )
  OPENCL_GET_PROC_ADDRESS(clEnqueueNDRangeKernel                  )
  OPENCL_GET_PROC_ADDRESS(clEnqueueTask                           )
  OPENCL_GET_PROC_ADDRESS(clEnqueueNativeKernel                   )
  OPENCL_GET_PROC_ADDRESS(clEnqueueMarker                         )
  OPENCL_GET_PROC_ADDRESS(clEnqueueWaitForEvents                  )
  OPENCL_GET_PROC_ADDRESS(clEnqueueBarrier                        )
  OPENCL_GET_PROC_ADDRESS(clGetExtensionFunctionAddress           )

  return true;
}

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

static bool OpenCLInitialize(
  OpenCLExecuteStruct* opencl_exec_struct
  )
{
  cl_int           ocl_err_code = CL_SUCCESS;
  cl_uint          num_platforms;
  cl_uint          num_devices;
  cl_platform_id   platforms[16];
  cl_device_id     devices[32];

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetPlatformIDs(16, platforms, &num_platforms)
    , return false);

  for (cl_uint index = 0; index < num_platforms; ++index)
  {
    OPENCL_SAFE_CALL0(
      ocl_err_code = func_clGetDeviceIDs(
        platforms[index], CL_DEVICE_TYPE_ALL, 32,
        devices, &num_devices)
      , return false);

    for (cl_uint index2 = 0; index2 < num_devices; ++index2)
    {
      if (opencl_exec_struct->m_num_devices < OCL_MAX_NUM_DEVICES)
      {
        opencl_exec_struct->m_device_list[
          opencl_exec_struct->m_num_devices++] = devices[index2];
      }
    }
  }

  OPENCL_SAFE_CALL0(
    opencl_exec_struct->m_context = func_clCreateContext(
      NULL,
      opencl_exec_struct->m_num_devices,
      opencl_exec_struct->m_device_list,
      NULL,
      NULL,
      &ocl_err_code)
    , return false);

  return true;
}

static bool OpenCLOutputDeviceInfo(cl_device_id device, FILE* file = stdout)
{
  cl_int           ocl_err_code = CL_SUCCESS;
  char             output_buffer[1024];
  size_t           param_value_size;
  cl_device_type   device_type;
  cl_uint          uint_value;
  size_t           size_t_value;

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_TYPE,
      sizeof(cl_device_type), &device_type,
      &param_value_size)
    , return false);
  switch (device_type)
  {
  case CL_DEVICE_TYPE_CPU:
    sprintf(output_buffer, "CL_DEVICE_TYPE_CPU");
    break;
  case CL_DEVICE_TYPE_GPU:
    sprintf(output_buffer, "CL_DEVICE_TYPE_GPU");
    break;
  case CL_DEVICE_TYPE_ACCELERATOR:
    sprintf(output_buffer, "CL_DEVICE_TYPE_ACCELERATOR");
    break;
  default:
    sprintf(output_buffer, "UNKNOWN TYPE");
    break;
  }
  fprintf(file, "  %-50s%s\n",
    "Device Type:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_VENDOR_ID,
    sizeof(cl_uint), &uint_value,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%xh\n",
    "Vendor ID:",
    uint_value);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_MAX_COMPUTE_UNITS,
    sizeof(cl_uint), &uint_value,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%d\n",
    "Max compute units:",
    uint_value);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_MAX_WORK_GROUP_SIZE,
    sizeof(size_t), &size_t_value,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%d\n",
    "Max work group size:",
    size_t_value);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_NAME,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Name:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_VENDOR,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Vendor:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DRIVER_VERSION,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Driver version:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_PROFILE,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Profile:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_VERSION,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Version:",
    output_buffer);

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clGetDeviceInfo(device, CL_DEVICE_EXTENSIONS,
    sizeof(output_buffer), &output_buffer,
    &param_value_size)
    , return false);
  fprintf(file, "  %-50s%s\n",
    "Extensions:",
    output_buffer);

  return true;
}

static bool OpenCLBuildProgramWithSource(
  cl_context           context,
  cl_uint              num_devices,
  const cl_device_id  *device_list,
  const char          *options,
  cl_uint              count,
  const char         **strings,
  const size_t        *lengths,
  cl_program          *program
  )
{
  cl_int           ocl_err_code = CL_SUCCESS;

  OPENCL_SAFE_CALL0(
    *program = func_clCreateProgramWithSource(
      context, count, strings, lengths, &ocl_err_code)
    , return false);

  ocl_err_code = func_clBuildProgram(
    *program, num_devices, device_list, options,
    NULL,
    NULL);

  if (CL_SUCCESS != ocl_err_code)
  {
    char   *error_message;
    size_t  size_message;

    OPENCL_LOG_ERROR(
      "Failed to build OpenCL program: (%d)%s\n",
      ocl_err_code,
      OpenCLGetErrorString(ocl_err_code));

    for (cl_uint index = 0; index < count; ++index)
    {
      OPENCL_SAFE_CALL0(
        ocl_err_code = func_clGetProgramBuildInfo(
          *program, device_list[index], CL_PROGRAM_BUILD_LOG,
          0, NULL, &size_message)
        , return false);

      error_message = (char*)malloc(size_message);

      ocl_err_code = func_clGetProgramBuildInfo(
        *program, device_list[index], CL_PROGRAM_BUILD_LOG,
        size_message, error_message, NULL);

      if (CL_SUCCESS == ocl_err_code)
      {
        OPENCL_LOG_ERROR(error_message);
      }

      free(error_message);
    }

    return false;
  }

  return true;
}

OpenCLWrapper* OpenCLWrapper::GenerateInstance(
  OpenCLExecuteStruct* opencl_exec_struct /* = NULL */
  )
{
  if (OCL_FUNC_LOAD_INIT == m_opencl_func_loading_status)
  {
    if (OpenCLGetProcAddress())
      m_opencl_func_loading_status = OCL_FUNC_LOAD_SUCCESS;
    else
      m_opencl_func_loading_status = OCL_FUNC_LOAD_FAILED;
  }

  if (OCL_FUNC_LOAD_SUCCESS != m_opencl_func_loading_status)
  {
    return NULL;
  }

  if (NULL == opencl_exec_struct)
  {
    opencl_exec_struct = new OpenCLExecuteStruct();
    if (!OpenCLInitialize(opencl_exec_struct))
    {
      delete opencl_exec_struct;
      opencl_exec_struct = NULL;
      return NULL;
    }
  }

  return new OpenCLWrapper(opencl_exec_struct);
}

OpenCLWrapper::OpenCLWrapper(OpenCLExecuteStruct* opencl_exec_struct)
  : m_opencl_exec_struct(opencl_exec_struct)
{
}

OpenCLWrapper::~OpenCLWrapper()
{
}

void OpenCLWrapper::OutputCLInfo(FILE* file /* = stdout */)
{
  unsigned int index = 0;
  for (; index < m_opencl_exec_struct->m_num_devices; ++index)
  {
    OpenCLOutputDeviceInfo(
      m_opencl_exec_struct->m_device_list[index]);
    fprintf(stdout, "\n\n");
  }
}

void OpenCLWrapper::SelfTestDemo()
{
  static const char *ocl_test_kernel =
    "__kernel void TestKernel    (__global short *src,         \n"
    "                             __global short *dst)         \n"
    "{                                                         \n"
    "  uint   globalIdx = get_global_id(0);                    \n"
    "  dst[globalIdx]   = src[globalIdx] + 155;                \n"
    "}                                                         \n";
  cl_int            ocl_err_code = CL_SUCCESS;
  cl_command_queue  command_queue;
  cl_program        program;
  cl_kernel         kernel_test;
  cl_mem            mem_input;
  cl_mem            mem_output;
  short            *buf_input;
  short            *buf_output;
  const size_t      buf_size = 102400;
  size_t            kernel_string_length;
  size_t            global_offset = 0;
  size_t            global_size   = buf_size / sizeof(short);
  size_t            local_size    = 64;

  OPENCL_SAFE_CALL0(
    command_queue = func_clCreateCommandQueue(
      m_opencl_exec_struct->m_context,
      m_opencl_exec_struct->m_device_list[0],
      0,
      &ocl_err_code)
    , return);
  OPENCL_SAFE_CALL0(
    mem_input = func_clCreateBuffer(
      m_opencl_exec_struct->m_context,
      CL_MEM_READ_ONLY,
      buf_size,
      NULL,
      &ocl_err_code)
    , return);
  OPENCL_SAFE_CALL0(
    mem_output = func_clCreateBuffer(
      m_opencl_exec_struct->m_context,
      CL_MEM_WRITE_ONLY,
      buf_size,
      NULL,
      &ocl_err_code)
    , return);

  kernel_string_length = strlen(ocl_test_kernel) + 1;
  if (!OpenCLBuildProgramWithSource(
    m_opencl_exec_struct->m_context,
    m_opencl_exec_struct->m_num_devices,
    m_opencl_exec_struct->m_device_list,
    NULL,
    1,
    &ocl_test_kernel,
    &kernel_string_length,
    &program))
  {
    return;
  }

  OPENCL_SAFE_CALL0(
    kernel_test = func_clCreateKernel(
      program,
      "TestKernel",
      &ocl_err_code);
    , return);
  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clSetKernelArg(
      kernel_test,
      0,
      sizeof(cl_mem),
      &mem_input)
    , return);
  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clSetKernelArg(
      kernel_test,
      1,
      sizeof(cl_mem),
      &mem_output);
  , return);

  buf_input  = new short[buf_size / sizeof(short)];
  buf_output = new short[buf_size / sizeof(short)];
  for (int index = 0; index < buf_size / sizeof(short); ++index)
  {
    buf_input[index] = 10000;
  }

  fprintf(stdout, "Input data:\n");
  for (int index = 0; index < 256; ++index)
  {
    fprintf(stdout, "%d ", buf_input[index]);
  }
  fprintf(stdout, "\n");

  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clEnqueueWriteBuffer(
      command_queue,
      mem_input,
      CL_TRUE,
      0,
      buf_size,
      buf_input,
      0,
      NULL,
      NULL)
    , return);
  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clEnqueueNDRangeKernel(
      command_queue,
      kernel_test,
      1,
      &global_offset,
      &global_size,
      &local_size,
      0,
      NULL,
      NULL)
    , return);
  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clEnqueueReadBuffer(
      command_queue,
      mem_output,
      CL_TRUE,
      0,
      1024,
      buf_output,
      0,
      NULL,
      NULL)
    , return);
  OPENCL_SAFE_CALL0(
    ocl_err_code = func_clFinish(command_queue)
    , return);

  fprintf(stdout, "Output data:\n");
  for (int index = 0; index < 256; ++index)
  {
    fprintf(stdout, "%d ", buf_output[index]);
  }
  fprintf(stdout, "\n");

  delete[] buf_input;
  delete[] buf_output;
}
