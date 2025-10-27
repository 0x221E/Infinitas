#include "virtual-machine/executor_cases/type_conversion.h"

#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{

    void typeConversion::IntegerToString(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, "__integer_to_string", context);
        auto integer = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), "__integer_to_string", context);
        std::string result = integer.str();
        context.m_Stack.Push(result);
    }
    
    void typeConversion::FloatToString(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, "__float_to_string", context);
        auto floatingPoint = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), "__float_to_string", context);
        std::string result = floatingPoint.str();
        context.m_Stack.Push(result);
    }
    
    void typeConversion::FloatToInteger(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, "__integer_to_string", context);
        auto integer = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), "__float_to_string", context);
        mp::mpz_int result = integer.convert_to<mp::mpz_int>();
        context.m_Stack.Push(result);
    }
}
