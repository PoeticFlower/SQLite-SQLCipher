struct OpenCLExecuteStruct;
class OpenCLWrapper
{
public:
  static OpenCLWrapper* GenerateInstance(
    OpenCLExecuteStruct* opencl_exec_struct = NULL
    );
  virtual              ~OpenCLWrapper();
  void                  OutputCLInfo(FILE* file = stdout);
  void                  SelfTestDemo();
protected:
  OpenCLWrapper(OpenCLExecuteStruct* opencl_exec_struct);
private:
  OpenCLExecuteStruct                     *m_opencl_exec_struct;
  static unsigned int                      m_opencl_func_loading_status;
};
