#pragma once

#include "virtual-machine/virtual_machine.h"
#include "virtual-machine/executor_cases/all.h"

/**
 * @todo I am not happy with this macro work-around, but I cannot think of a better one right now.
 * @todo Populate dispatcher during compile-time.
 */

namespace vm
{
#define REGISTER_ALL_CASES(dispatcher) \
    {   \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_INT_ADD, &ArithmeticCases::IntegerAdd); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_FLOAT_ADD, &ArithmeticCases::FloatAdd); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_INT_SUB, &ArithmeticCases::IntegerSub); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_FLOAT_SUB, &ArithmeticCases::FloatSub); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_INT_MUL, &ArithmeticCases::IntegerMul); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_FLOAT_MUL, &ArithmeticCases::FloatMul); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_INT_DIV, &ArithmeticCases::IntegerDiv); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_FLOAT_DIV, &ArithmeticCases::FloatDiv); \
        \
    dispatcher.Register(shared::Opcodes::OP_BLOCK_START, &BlockCases::StartBlock); \
    dispatcher.Register(shared::Opcodes::OP_BLOCK_END, &BlockCases::EndBlock); \
        \
    dispatcher.Register(shared::Opcodes::OP_LOAD_CONSTANT, &NativeCases::LoadConstant); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_PRINT, &NativeCases::Print); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_EXIT, &NativeCases::Exit); \
        \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_INT_TO_STR, &typeConversion::IntegerToString); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_FLOAT_TO_STR, &typeConversion::FloatToString); \
        \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_UNARY_NEGATE_INT, &UnaryCases::NegateInteger); \
    dispatcher.Register(shared::Opcodes::OP_NATIVE_UNARY_NEGATE_FLOAT, &UnaryCases::NegateFloat); \
        \
    dispatcher.Register(shared::Opcodes::OP_DECLARE_VARIABLE, &VariableCases::DeclareVariable); \
    dispatcher.Register(shared::Opcodes::OP_ASSIGN_VARIABLE, &VariableCases::AssignVariable); \
    dispatcher.Register(shared::Opcodes::OP_ACCESS_VARIABLE, &VariableCases::AccessVariable); \
    }
}

