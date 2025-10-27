#include "virtual-machine/executor_cases/native.h"
#include "virtual-machine/executor_cases/exec_utils.h"
#include "virtual-machine/exception_runtime.h"

namespace vm
{ 
    
    void NativeCases::LoadConstant(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_Stack.Push(context.m_Constants.GetConstant(instruction.m_Value));
    }
    
    /**
     * @todo This function is not te most efficient way yet.
     */
    void NativeCases::Print(VMContext &context,const shared::Instruction& instruction)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(instruction.m_Value, "__print", context);

        /** @todo Implement stack logic that allows you to peek the elements, print in a single loop, then pop all N number of elements. */
        std::vector<std::string> params;
        params.reserve(instruction.m_Value);
        
        std::ostream &os = std::cout;
        for (std::size_t i = 0; i < instruction.m_Value; i++)
        {
            auto param = ExecutorUtils::TypeCheck<std::string>(context.m_Stack.PopMove(), "__print", context);

            params.push_back(param);
        }

        for(auto i = params.rbegin(); i != params.rend(); ++i)
        {
            os << *i;
        }

        os << std::endl
           << std::flush;
    }

    void NativeCases::Exit(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_IsHalted = true;
    }
}
