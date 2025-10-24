#pragma once

#include <array>
#include <shared/opcodes.h>

namespace shared
{
    struct Instruction;
}

namespace vm
{
    struct VMContext;

    using ExecutionCase = void(*)(VMContext&, const shared::Instruction&);

    class ExecutionDispatcher
    {
    public:
        ExecutionDispatcher();
        
    public:
        void Dispatch(VMContext& context, const shared::Instruction& instruction);

        /**
         * @warning Since Opcodes is a uint8_t, the maximum number of unique opcodes is 256 and therefore this function
         *          is noexcept. If this is changed, both m_Table size and Opcodes must be updated to handle errors.
         */
        void Register(shared::Opcodes opcode, void(*executor)(VMContext&, const shared::Instruction&)) noexcept;

    private:
        std::array<ExecutionCase, 256> m_Table;
        std::size_t m_LogicalSize = 0;
    };
}
