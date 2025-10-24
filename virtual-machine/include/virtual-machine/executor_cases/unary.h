#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace UnaryCases
    {
        void NegateInteger(VMContext& context, const shared::Instruction& instruction);
        void NegateFloat(VMContext& context, const shared::Instruction& instruction);
    };
}
