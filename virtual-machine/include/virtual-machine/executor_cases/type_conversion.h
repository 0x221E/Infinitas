#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace typeConversion
    {
        void IntegerToString(VMContext& context, const shared::Instruction& instruction);
        void IntegerToFloat(VMContext& context, const shared::Instruction& instruction);
        void FloatToString(VMContext& context, const shared::Instruction& instruction);
        void FloatToInteger(VMContext& context, const shared::Instruction& instruction);
        void StringToFloat(VMContext& context, const shared::Instruction& instruction);
        void StringToInteger(VMContext& context, const shared::Instruction& instruction);
    }
}
