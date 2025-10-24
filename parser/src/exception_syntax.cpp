#include "parser/exception_syntax.h" 

namespace parser
{
  SyntaxException::SyntaxException(shared::ExceptionSeverity severity, SyntaxExceptionCodes code, std::string message) : shared::Exception(shared::ExceptionLocation::EX_TYPE_PARSER, severity, static_cast<uint64_t>(code), message)
  {
    
  }

  std::string SyntaxException::ToString() const
  {
    std::string result = "SyntaxError(" + std::to_string(GetErrorCode()) + "): " + GetMessage();
    return result;
  }
}
