#include "virtual-machine/executor_cases/arithmetic.h"

#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{
    void ArithmeticCases::IntegerAdd(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_add function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        
        if(!b || !a)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_add function requires 2 ___integer types!");
            return;
        }

        mp::mpz_int result = a.value() + b.value(); 

        context.m_Stack.Push(std::move(result));
    }
    
    void ArithmeticCases::FloatAdd(VMContext &context, const shared::Instruction& instruction)
    {
         if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_add function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());

        if(!b.has_value() || !a.has_value())
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_add function requires 2 ___float types!");
            return;
        }

        mp::mpf_float result = a.value() + b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerSub(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_sub function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        
        if(!b || !a)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_sub function requires 2 ___integer types!");
            return;
        }

        mp::mpz_int result = a.value() - b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatSub(VMContext &context, const shared::Instruction& instruction)
    {
         if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_sub function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());

        if(!b.has_value() || !a.has_value())
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_sub function requires 2 ___float types!");
            return;
        }

        mp::mpf_float result = a.value() - b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerMul(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_mul function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        
        if(!b || !a)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_mul function requires 2 ___integer types!");
            return;
        }

        mp::mpz_int result = a.value() * b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatMul(VMContext &context, const shared::Instruction& instruction)
    {
         if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_mul function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());

        if(!b.has_value() || !a.has_value())
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_mul function requires 2 ___float types!");
            return;
        }

        mp::mpf_float result = a.value() * b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::IntegerDiv(VMContext &context, const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_mul function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        
        if(!b || !a)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_mul function requires 2 ___integer types!");
            return;
        }

        mp::mpz_int result = a.value() / b.value(); 

        context.m_Stack.Push(std::move(result));
    }

    void ArithmeticCases::FloatDiv(VMContext &context, const shared::Instruction& instruction)
    {
         if (context.m_Stack.Size() < 2)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_mul function requires at least 2 elements loaded on stack.");
        }

        auto b = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());
        auto a = TypeCheck<mp::mpf_float>(context.m_Stack.PopMove());

        if(!b.has_value() || !a.has_value())
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__float_mul function requires 2 ___float types!");
            return;
        }

        mp::mpf_float result = a.value() / b.value(); 

        context.m_Stack.Push(std::move(result));
    }
}
