#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace BlockCases
    {
        void StartBlock(VMContext& context, const shared::Instruction& instruction);
        void EndBlock(VMContext&context, const shared::Instruction& instruction);
    };
}
