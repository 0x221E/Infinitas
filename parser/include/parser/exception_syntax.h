#pragma once

#include "shared/exception.h"

namespace parser
{
  enum class SyntaxExceptionCodes : std::uint64_t
  {
    EX_SYNTAX_UNKNOWN = 0,
    EX_SYNTAX_MISSING_SEMICOLON = 1,
    EX_SYNTAX_MISSING_LEFTBRACE = 2,
    EX_SYNTAX_MISSING_RIGHTBRACE = 3,
    EX_SYNTAX_MISSING_LEFTPAREN = 4,
    EX_SYNTAX_MISSING_RIGHTPAREN = 5,
    EX_SYNTAX_MISSING_EQUAL = 6,
    EX_SYNTAX_MISSING_IDENTIFIER = 7
  };

  class SyntaxException : public shared::Exception
  {
  public:
    using ExceptionCodeType = SyntaxExceptionCodes;

  public:
    SyntaxException(shared::ExceptionSeverity severity, SyntaxExceptionCodes code, std::string message);
    ~SyntaxException() { }

  public:
    std::string ToString() const override;
  };
}
