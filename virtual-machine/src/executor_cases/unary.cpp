#include "virtual-machine/executor_cases/unary.h"

#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{
    void UnaryCases::NegateInteger(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, "__negate_integer", context);

        auto value = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
    
        mp::mpz_int result = value * -1;
        context.m_Stack.Push(result);
    }

    void UnaryCases::NegateFloat(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, "__negate_float", context);
        
        auto value = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.ReferenceBack(), __func__, context);
    
        mp::mpf_float result = value * -1;
        context.m_Stack.Push(result);
    }
}
