#pragma once

#include "shared/exception.h"

namespace lexer
{
  enum class LexerExceptionCodes : uint64_t
  {
    EX_LEX_UNKNOWN = 0,
    EX_LEX_CHAR_UNKNOWN = 1,
    EX_LEX_CHAR_MISPLACED = 2
  };

  class LexerException : public shared::Exception
  {
  public:
    using ExceptionCodeType = LexerExceptionCodes;

  public:
    LexerException(shared::ExceptionSeverity severity, LexerExceptionCodes code, std::string message);

  public:
    std::string ToString() const override;
  };
}
