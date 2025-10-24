#pragma once

#include "opcodes.h"
#include "objects/all.h"

#include <vector>
#include <memory>
#include <optional>
#include <variant>

namespace shared
{
    
    struct Instruction
    {
        Opcodes m_Opcode;
        std::size_t m_Value;
        std::size_t m_Value2;

        Instruction(Opcodes op) : m_Opcode{op}, m_Value{0}, m_Value2{0} { }
        Instruction(Opcodes op, const std::size_t& value) : m_Opcode{op}, m_Value{value}, m_Value2{0} { }
        Instruction(Opcodes op, const std::size_t& value, const std::size_t& value2) : m_Opcode{op}, m_Value{value}, m_Value2{value2} { }

        Instruction() = default;
    };

}