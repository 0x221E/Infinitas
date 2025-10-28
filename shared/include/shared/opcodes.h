/**
 * @file opcodes.hpp
 * @author 0x221E
 *
 * File contains opcodes that are used during the compilation phase
 * to convert parsed data to instructions, and on the virtual machine
 * to execute the compiled ohcompiled bytecode instructions.
 */

#pragma once

#include <cstdint>


namespace shared
{
    enum Opcodes : std::uint8_t
    {
        OP_LOAD_CONSTANT = 0x01,
        OP_DECLARE_VARIABLE = 0x02,
        OP_ASSIGN_VARIABLE = 0x03,
        OP_ACCESS_VARIABLE = 0x04,
        OP_UNARY_NEGATION = 0x05,
        OP_BINARY_ADDITION = 0x06,
        OP_BINARY_SUBSTRACTION = 0x07,
        OP_BINARY_DIVISION = 0x08,
        OP_BINARY_MULTIPLICATION = 0x09,
        OP_FUNCTION_CALL = 0x10,
        OP_NATIVE_PRINT = 0x11, 
        OP_NATIVE_EXIT = 0x12,
        OP_NATIVE_INT_ADD = 0x13,
        OP_NATIVE_FLOAT_ADD = 0x14,
        OP_NATIVE_INT_SUB = 0x15,        
        OP_NATIVE_FLOAT_SUB = 0x16,
        OP_NATIVE_INT_MUL = 0x17,
        OP_NATIVE_FLOAT_MUL = 0x18,
        OP_NATIVE_INT_DIV = 0x19,
        OP_NATIVE_FLOAT_DIV = 0x20,
        OP_NATIVE_INT_TO_STR = 0x21,
        OP_NATIVE_INT_TO_FLOAT = 0x22,
        OP_NATIVE_FLOAT_TO_STR = 0x23,
        OP_NATIVE_FLOAT_TO_INT = 0x24,
        OP_NATIVE_STR_TO_FLOAT = 0x25,
        OP_NATIVE_STR_TO_INT = 0x26,
        OP_NATIVE_UNARY_NEGATE_INT = 0x27,
        OP_NATIVE_UNARY_NEGATE_FLOAT = 0x28,
        OP_BLOCK_START = 0x29,
        OP_BLOCK_END = 0x30,
    };
}
