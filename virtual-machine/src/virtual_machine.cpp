/**
 * @file virtual_machine.cpp
 * @author 0x221E
 */

#include "virtual-machine/virtual_machine.h"
#include <shared/opcodes.h>

#include <shared/logger.h>
#include <shared/objects/base_object.h>
#include "register.h"

namespace vm
{

    VirtualMachine::VirtualMachine()
       : m_Instructions{0}, m_Environment{}, m_InstructionPointer{0}, m_VMContext{m_Environment, m_Constants, m_Stack, m_IsHalted, m_ErrorContext}
    {
    }

    void VirtualMachine::Register()
    {
        REGISTER_ALL_CASES(m_ExecutionDispatcher)
    }

    void VirtualMachine::Dispatch(const shared::Instruction& instruction)
    {
        m_ExecutionDispatcher.Dispatch(m_VMContext, instruction);
    }

    void VirtualMachine::Interpret(VMInterpretProperties &props)
    {
        m_Instructions.reserve(m_Instructions.capacity() * 2); /** @todo Implement optimization. */
        m_Instructions.insert(m_Instructions.end(), std::make_move_iterator(props.m_Instructions.begin()), std::make_move_iterator(props.m_Instructions.end()));
        m_Constants.ReplaceConstants(std::move(props.m_Constants));

        VMLoop();
    }

    bool VirtualMachine::IsHalted() const noexcept
    {
        return m_IsHalted;
    }
  
    void VirtualMachine::VMLoop()
    {
        while (m_InstructionPointer < m_Instructions.size() && !m_IsHalted)
        {
            auto &instruction = m_Instructions[m_InstructionPointer];
            Dispatch(instruction);
            LOG_DEBUG("[VM] {:#x} executed.", static_cast<uint8_t>(instruction.m_Opcode)); /** @todo Implement better debugging for only debug build. */
            m_InstructionPointer++;
        }
    }

}
