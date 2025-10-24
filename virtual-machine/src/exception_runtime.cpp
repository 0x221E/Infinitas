#include "virtual-machine/exception_runtime.h"

namespace vm
{ 
  RuntimeException::RuntimeException(shared::ExceptionSeverity severity, RuntimeExceptionCodes code, std::string message) : shared::Exception(shared::ExceptionLocation::EX_TYPE_RUNTIME, severity, static_cast<uint64_t>(code), message)
  {
    
  }

  std::string RuntimeException::ToString() const
  {
    std::string result = "RuntimeError (" + std::to_string(GetErrorCode()) + "): " + GetMessage();
    return result;
  }
}
