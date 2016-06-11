#include "../common/common.h"
#include "../common/opencl_wrapper.h"
#include "../common/threading_wrapper.h"

class OpenCLTestThread : public ThreadingWrapper
{
public:
  virtual void ThreadMain();
};

void OpenCLTestThread::ThreadMain()
{
  OpenCLWrapper *opencl_demo = OpenCLWrapper::GenerateInstance();
  if (NULL == opencl_demo)
  {
    printf("Failed to generate OpenCLWrapper!\n");
    exit (-1);
  }
  opencl_demo->OutputCLInfo();
  opencl_demo->SelfTestDemo();
}

int main(int argc, char** argv)
{
  ThreadingWrapper *opencl_test_thread = new OpenCLTestThread();
  opencl_test_thread->Start();
  opencl_test_thread->Stop();

  return 0;
}
