#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace NativeCases
    {
        void IntegerToString(VMContext& context, const shared::Instruction& instruction);
        void LoadConstant(VMContext& context, const shared::Instruction& instruction);
        void Print(VMContext& context, const shared::Instruction& instruction);
        void Exit(VMContext& context, const shared::Instruction& instruction);
    };
}
