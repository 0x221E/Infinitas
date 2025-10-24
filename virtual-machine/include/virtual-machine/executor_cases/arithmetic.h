#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace ArithmeticCases
    {
        void IntegerAdd(VMContext& context, const shared::Instruction& instruction);
        void FloatAdd(VMContext& context, const shared::Instruction& instruction);
        void IntegerSub(VMContext& context, const shared::Instruction& instruction);
        void FloatSub(VMContext& context, const shared::Instruction& instruction);
        void IntegerMul(VMContext& context, const shared::Instruction& instruction);
        void FloatMul(VMContext& context, const shared::Instruction& instruction);
        void IntegerDiv(VMContext& context, const shared::Instruction& instruction);
        void FloatDiv(VMContext& context, const shared::Instruction& instruction);
    };
}
