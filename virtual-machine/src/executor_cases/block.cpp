#include "virtual-machine/executor_cases/block.h"

namespace vm
{
    void BlockCases::StartBlock(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_Environment.AddScope();
    }

    void BlockCases::EndBlock(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_Environment.RemoveScope();
    }
}
