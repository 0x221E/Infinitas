#include "compiler/exception_compiler.h"
#include "shared/exception.h"

namespace compiler
{
  CompilerException::CompilerException(shared::ExceptionSeverity severity, CompilerExceptionCodes code, std::string message)
    : shared::Exception(shared::ExceptionLocation::EX_TYPE_COMPILER, severity, static_cast<uint64_t>(code), message)
  {

  }

  std::string CompilerException::ToString() const
  {
    std::string result = "CompileError(" + std::to_string(GetErrorCode()) + "): " + GetMessage();
    return result;
  }
}
