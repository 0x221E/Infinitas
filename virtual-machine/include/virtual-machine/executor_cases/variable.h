#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace VariableCases
    {
        void DeclareVariable(VMContext& context, const shared::Instruction& instruction);
        void AssignVariable(VMContext& context, const shared::Instruction& instruction);
        void AccessVariable(VMContext& context, const shared::Instruction& instruction);
    }
}
