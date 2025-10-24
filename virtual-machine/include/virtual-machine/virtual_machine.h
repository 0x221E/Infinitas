/**
 * @file virtual_machine.h
 * @author 0x221E
 *
 * The base class of ohvm.
 */
#pragma once

#include "shared/error_context.h"
#include "virtual-machine/stack_container.h"
#include "virtual-machine/execution_dispatcher.h"
#include "virtual-machine/exception_runtime.h"
#include "environment.h"
#include <vector>

#include <shared/instruction_container.h>
#include <shared/constant_pool.h>

namespace vm
{
    struct VMContext
    {
        Environment& m_Environment;
        shared::ConstantPool& m_Constants;
        StackContainer& m_Stack;
        bool& m_IsHalted;
        shared::ErrorContext<RuntimeException>& m_ErrorContext;
    };

    struct VMInterpretProperties
    {
        std::vector<shared::Instruction>&& m_Instructions;
        std::vector<Types>&& m_Constants;
    };

    class VirtualMachine
    {
    public:
        VirtualMachine();
    
    public:
        void Register();
        void Interpret(VMInterpretProperties& props);
        void Dispatch(const shared::Instruction& instruction);
        bool IsHalted() const noexcept;

    private:
        std::vector<shared::Instruction> m_Instructions;
        ExecutionDispatcher m_ExecutionDispatcher;
        Environment m_Environment;
        shared::ConstantPool m_Constants;
        StackContainer m_Stack;
        size_t m_InstructionPointer;

        shared::ErrorContext<RuntimeException> m_ErrorContext;
        VMContext m_VMContext;
  
        bool m_IsHalted = false;
    
    private:
        void VMLoop();
    };
}
