#pragma once

#include "shared/exception.h"
#include <cstdint>

namespace compiler
{
  enum class CompilerExceptionCodes : std::uint64_t
  {
    EX_COMPILER_UNKNOWN = 0,
    EX_COMPILER_OP_NOT_FOUND = 1,
    EX_COMPILER_VAR_IMPROPER = 2,
    EX_RESOLUTION_VAR_RE_DECL = 3,
    EX_RESOLUTION_VAR_NOT_FOUND = 4,
    EX_RESOLUTION_SYMBOL_ERR = 5  
};

  class CompilerException : public shared::Exception
  {
  public: 
    using ExceptionCodeType = CompilerExceptionCodes;
  
  public:
    CompilerException(shared::ExceptionSeverity severity, CompilerExceptionCodes code, std::string message);
    ~CompilerException() { }
  public:
    std::string ToString() const override;
  };
}
