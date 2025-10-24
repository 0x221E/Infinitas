#include "tokenizer/exception_lexer.h"

namespace lexer
{
  LexerException::LexerException(shared::ExceptionSeverity severity, LexerExceptionCodes code, std::string message)
    : shared::Exception(shared::ExceptionLocation::EX_TYPE_LEXER, severity, static_cast<uint64_t>(code), message)
  {
  
  }

  std::string LexerException::ToString() const
  {
    std::string result = "Tokenizer Error(" + std::to_string(GetErrorCode()) + "): " + GetMessage();
    return result;
  }
}
