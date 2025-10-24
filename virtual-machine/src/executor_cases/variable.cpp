#include "virtual-machine/executor_cases/variable.h"

namespace vm
{
    void VariableCases::DeclareVariable(VMContext &context, const shared::Instruction& instruction)
    {
        context.m_Environment.AddVariable(instruction.m_Value, instruction.m_Value2, std::move(context.m_Stack.PopMove()));
    }

    void VariableCases::AssignVariable(VMContext &context, const shared::Instruction& instruction)

    {
        auto &obj = context.m_Environment.UnqualifiedLookup(instruction.m_Value, instruction.m_Value2);
        obj = std::move(context.m_Stack.PopMove());
    }
    
    void VariableCases::AccessVariable(VMContext &context, const shared::Instruction& instruction)
    {
        auto &object = context.m_Environment.UnqualifiedLookup(instruction.m_Value, instruction.m_Value2);

        context.m_Stack.Push(std::visit([](auto&& obj) -> Types
        {
            using types = std::remove_reference_t<decltype(obj)>;

            if constexpr(std::is_same_v<types, ObjectPtr>)
            {
                return obj->Clone();
            }
            else
            {
                return obj;
            }

        }, object));
    }
}
