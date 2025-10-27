#include "virtual-machine/executor_cases/arithmetic.h"

#include "virtual-machine/exception_runtime.h"
#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{
    void ArithmeticCases::IntegerAdd(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__integer_add", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        
        mp::mpz_int result = a + b; 

        context.m_Stack.Push(std::move(result));
    }
    
    void ArithmeticCases::FloatAdd(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__float_add", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);

        mp::mpf_float result = a + b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerSub(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__integer_sub", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        
        mp::mpz_int result = a - b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatSub(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__float_sub", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);

        mp::mpf_float result = a - b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerMul(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__integer_mul", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        
        mp::mpz_int result = a * b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatMul(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__float_mul", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);

        mp::mpf_float result = a * b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerDiv(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__integer_div", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpz_int>(context.m_Stack.PopMove(), __func__, context);
       
        if(b == 0)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_ARITHMETIC_ZERODIVISION, "__integer_div: Division by zero!");
        }

        mp::mpz_int result = a / b; 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatDiv(VMContext &context, const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(2, "__integer_div", context);

        auto b = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);
        auto a = ExecutorUtils::TypeCheck<mp::mpf_float>(context.m_Stack.PopMove(), __func__, context);

        if(b == 0)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_ARITHMETIC_ZERODIVISION, "__float_div: Division by zero!");
        }

        mp::mpf_float result = a / b; 

        context.m_Stack.Push(std::move(result));
    }
}
