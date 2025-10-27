#pragma once

#include <expected>
#include <variant>
#include <shared/objects/base_object.h>
#include "virtual-machine/exception_runtime.h"
#include "virtual-machine/virtual_machine.h"

namespace vm
{
    class ExecutorUtils
    {
    public:
        static void AssertStackSizeNotEqual(std::size_t size, std::string function, VMContext& context);    
        static void AssertStackSizeGreaterEqual(std::size_t size, std::string function, VMContext& context);    

       /**
       * @brief A utility function that checks the type against the supplied template argument.
       * @returns If matches the supplied type, returns the object as the type specified in template arguments, otherwise a boolean indicating error.
       */
        template<typename ExpectedType, typename T>
        static ExpectedType TypeCheck(T&& obj, const std::string& function_name, VMContext& context)
        {
            auto* value = std::get_if<ExpectedType>(&obj);
            if (value)
            {
                return *value;
            }
            
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_NATIVE_PARAMETER_INVALID, function_name + ": argument mismatch. Check specifications.");
            throw;
        }
    };   

    template<typename SrcType, typename DstType>
    static void ConvertTo(const std::string& function_name, VMContext& context)
    {
        ExecutorUtils::AssertStackSizeGreaterEqual(1, function_name, context);

        auto a = ExecutorUtils::TypeCheck<SrcType>(context.m_Stack.PopMove(), function_name, context);
        
        DstType result = a;

        context.m_Stack.Push(result);
    }
}
