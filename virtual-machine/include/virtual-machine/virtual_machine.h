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
    /**
     * @brief Context that is passed to executor cases during each dispatch.
     * * This context holds a reference to all values which would need to be accessed
     * * during the opcode execution phase.
     */
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
    
    /**
     * @brief The interpreter
     */
    class VirtualMachine
    {
    public:
        VirtualMachine();
        ~VirtualMachine() = default;

        /** Deleted move/copy constructor/assignment */
        VirtualMachine(VirtualMachine&& other) = delete;
        VirtualMachine(const VirtualMachine& other) = delete;
        
        VirtualMachine& operator=(VirtualMachine&& other) = delete;
        VirtualMachine& operator=(const VirtualMachine& other) = delete;

    public:
        /**
         * @brief Registers executor cases.
         */
        void Register();
        
        /**
         * @brief Initializes the virtual machine, and executes provided bytecode.
         * * Moves necessary runtime constructs (Constant Pool, Instructions) into the VM 
         * * and triggers the interpretation process.
         * @param props A reference to a VMInterpretProperties struct, which holds 
         * the constant pool, and instruction set to be executed.
         * @returns std::expected<void, bool> An empty (void) result on success, or 
         * a boolean (true) indicator on failre (e.g., RuntimeException).
         */
        std::expected<void, bool> Interpret(VMInterpretProperties& props);
    
        /**
         * @brief Dispatches the provided instruction to the appropriate executor case.
         * @param instruction A const reference to the shared::Instruction struct to be dispatched.
         */
        void Dispatch(const shared::Instruction& instruction);

        /**
         * @brief Fetches the error state of the Virtual Machine.
         * @returns boolean value indicating whether the last Virtual Machine resulted in an error.
         */
        bool IsHalted() const noexcept;

        /**
         * @brief Resets the ErrorContext of the Virtual Machine. 
         * @warning ErrorContext is reset on each failed run during REPL.
         */
        void ResetErrors() noexcept;
        
        /**
         * @brief Takes a snapshot of the current state of the Virtual Machine.
         * @returns Newly constructed VMSnapshot struct. 
         */
        VMSnapshot TakeSnapshot();
        
        /**
         * @brief Recovers the Virtual Machine from an error state.
         * * Resets errors, and restores the state to the snapshot.
         * @param snapshot The state to be restored.
         */
        void Recover(const VMSnapshot& snapshot);

        /**
         * @brief Recovers the Virtual Machine to the supplied snapshot.
         * * Populates Virtual Machine internals from the snapshot.
         * @param snapshot The state to be restored.
         */
        void RecoverFromSnapshot(const VMSnapshot& snapshot);

    private:
        /** @brief Holds all instructions of execution loop. */
        std::vector<shared::Instruction> m_Instructions;

        /** @brief Dispatching mechanism for executor cases. */
        ExecutionDispatcher m_ExecutionDispatcher;

        /** @brief Constants table which are populated during compilation and provided to the Virtual Machine. */
        shared::ConstantPool m_Constants;

        /** @brief Points to the next shared::Instruction in line to be executed inside m_Instructions. */
        size_t m_InstructionPointer;

        /** @brief Holds user-defined constructs. Access to user-defined constructs is O(1) during runtime. */
        Environment m_Environment;

        /** @brief Holds temporary constructs. When an operation is to be performed on language constructs, they are temporarily added here. */
        StackContainer m_Stack;

        /** @brief Error construct which holds the error state of the virtual machine. */
        shared::ErrorContext<RuntimeException> m_ErrorContext;

        /** 
         * @brief Passed as a reference to each executor case, holds references to all relevant Virtual Machine components. 
         * @details View VMContext documentation for further information. 
         */
        VMContext m_VMContext;
  
        /** 
         * @brief Variable indicating the error state of the current Virtual Machine instance.
         * @warning Virtual Machine Loop does not continue or run again if error is found (set to true).
         * @warning This state should not be changed manually, the error state must be reset through recovery functions.
         */
        bool m_IsHalted = false;
    
    private:
        /**
         * @brief The main loop of the interpreter.
         * * After populating the internal variables, this function executes the instructions in line.
         * @warning This function is called by the Interpret function.
         */
        void VMLoop();
    };
}
