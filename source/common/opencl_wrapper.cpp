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
