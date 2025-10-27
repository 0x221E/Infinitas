#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{
    void ExecutorUtils::AssertStackSizeNotEqual(std::size_t size, std::string function, VMContext& context)
    {
        if (context.m_Stack.Size() == size) return;
        context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_UNKNOWN, function + ": stack size must be equal to " + std::to_string(size));
        return;
    }

    void ExecutorUtils::AssertStackSizeGreaterEqual(std::size_t size, std::string function, VMContext& context)
    {
        if (context.m_Stack.Size() >= size) return;
        context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_UNKNOWN, function + ": stack size must greater than " + std::to_string(size));
        return;
    }
}
