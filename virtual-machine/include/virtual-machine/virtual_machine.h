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
#include <expected>

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

    /**
     * @brief A props struct for the VirtualMachine::Interpret function.
     * * Implemented so that the Interpret function is more clear.
     */
    struct VMInterpretProperties
    {
        std::vector<shared::Instruction> m_Instructions;
        std::vector<Types> m_Constants;
    };
    
    /**
     * @brief Used to keep track of a state of the virtual machine.
     * * Copies made through this construct can be loaded to an instance of VirtualMachine at any time.
     *
     * @warning Instead of copying the entire VirtualMachine, this construct should be used!
     */ 
    struct VMSnapshot
    {
        std::vector<shared::Instruction> m_Instructions;
        shared::ConstantPool m_Constants;
        Environment m_Environment;
        StackContainer m_Stack;
        size_t m_InstructionPointer;
    };

    class VirtualMachine
    {
    public:
        VirtualMachine();

    public:
        void Register();
        std::expected<void, bool> Interpret(VMInterpretProperties& props);
        void Dispatch(const shared::Instruction& instruction);
        bool IsHalted() const noexcept;
        void ResetErrors() noexcept;
        
        VMSnapshot TakeSnapshot();
        void Recover(const VMSnapshot& snapshot);
        void RecoverFromSnapshot(const VMSnapshot& snapshot);

    private:
        std::vector<shared::Instruction> m_Instructions;
        ExecutionDispatcher m_ExecutionDispatcher;
        shared::ConstantPool m_Constants;
        size_t m_InstructionPointer;
        Environment m_Environment;
        StackContainer m_Stack;

        shared::ErrorContext<RuntimeException> m_ErrorContext;
        VMContext m_VMContext;
  
        bool m_IsHalted = false;
    
    private:
        void VMLoop();
    };
}
