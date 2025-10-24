#include "virtual-machine/executor_cases/native.h"
#include "virtual-machine/executor_cases/exec_utils.h"
#include "virtual-machine/exception_runtime.h"

namespace vm
{
    void NativeCases::IntegerToString(VMContext& context, const shared::Instruction& instruction)
    {
        if(context.m_Stack.Size() < 1)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_to_string function requires an ___integer parameter.");
            return;
        }
        
        auto a = TypeCheck<mp::mpz_int>(context.m_Stack.PopMove());
        
        if(!a.has_value())
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__integer_to_string function requires an ___integer parameter.");
            return;
        }
        
        context.m_Stack.Push(a.value().str());
    }
    
    void NativeCases::LoadConstant(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_Stack.Push(context.m_Constants.GetConstant(instruction.m_Value));
    }
    
    /**
     * @todo This function is not te most efficient way yet.
     */
    void NativeCases::Print(VMContext &context,const shared::Instruction& instruction)
    {
        if (context.m_Stack.Size() < instruction.m_Value)
        {
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "Stack size mismatch with supplied argument count for __print function.");
            return;
        }

        /** @todo Implement stack logic that allows you to peek the elements, print in a single loop, then pop all N number of elements. */
        std::vector<std::string> params;
        params.reserve(instruction.m_Value);
        
        std::ostream &os = std::cout;
        for (std::size_t i = 0; i < instruction.m_Value; i++)
        {
            auto param = TypeCheck<std::string>(context.m_Stack.PopMove());

            if(!param)
            {
                context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_STACK_INVALID, "__print function arguments must be of type ___string.");
                return;
            }

            params.push_back(param.value());
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
