#include "virtual-machine/execution_dispatcher.h"

#include "virtual-machine/virtual_machine.h"

namespace vm
{
    ExecutionDispatcher::ExecutionDispatcher()
    {

    }

    void ExecutionDispatcher::Dispatch(VMContext& context, const shared::Instruction& instruction)
    {
        auto& func = m_Table[instruction.m_Opcode];
        if(func)
        {
            func(context, instruction);
        }
        else
        {
            /** 
             * VM must exit with error in this case.
             * 
             * @todo Decide on error handling strategy here, as this is an extraordinary case.
             */
            throw std::runtime_error("[ExecutionDispatcher] Operation could not be dispatched! Reason: Operation does not exist.");
        }
    }

    void ExecutionDispatcher::Register(shared::Opcodes opcode, void(*executor)(VMContext&, const shared::Instruction&)) noexcept
    {
        static_assert(sizeof(shared::Opcodes) == sizeof(uint8_t), "Opcodes must be of size 256, if this is changed, update m_Table size.");
        m_Table[opcode] = executor;
    }

}
