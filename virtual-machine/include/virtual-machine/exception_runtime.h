#pragma once

#include <shared/exception.h>

namespace vm
{
    enum class RuntimeExceptionCodes : std::uint64_t
    {
        EX_RUNTIME_UNKNOWN = 0,
        EX_RUNTIME_STACK_INVALID = 1,
        EX_NATIVE_PARAMETER_INVALID = 2,
        EX_ARITHMETIC_ZERODIVISION = 3,
        EX_RUNTIME_CONVERSION_ERROR = 4
    };

    class RuntimeException : public shared::Exception
    {
    public:
        using ExceptionCodeType = RuntimeExceptionCodes;

    public:
        RuntimeException(shared::ExceptionSeverity severity, RuntimeExceptionCodes code, std::string message);
    
    public:
        std::string ToString() const override;
    };
}
