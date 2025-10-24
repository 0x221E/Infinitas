#include "virtual-machine/executor_cases/unary.h"

#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{
    void UnaryCases::NegateInteger(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 1)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "Negation operation requires the stack to have at least 1 element. Negation cannot be attempted not on a object.");
        }

        auto value = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
    
        if(!value)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__negate_integer function requires type ___integer as argument!");
        }
              
        mp::mpz_int result = value.value() * -1;
        context.m_Stack.Push(result);
    }

    void UnaryCases::NegateFloat(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 1)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "Negation operation requires the stack to have at least 1 element. Negation cannot be attempted not on a object.");
        }

        auto value = TypeCheck<mp::mpf_float>(context.m_Stack.ReferenceBack());
    
        if(!value)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__negate_float function requires type ___float as argument!");
        }
        
        mp::mpf_float result = value.value() * -1;
        context.m_Stack.Push(result);
    }
}
