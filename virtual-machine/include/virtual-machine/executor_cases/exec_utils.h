#pragma once

#include <expected>
#include <type_traits>
#include <variant>
#include <shared/objects/base_object.h>
#include "virtual-machine/exception_runtime.h"
#include "virtual-machine/virtual_machine.h"

namespace vm
{
    class ExecutorUtils
    {
    public:
        static void AssertStackSizeNotEqual(std::size_t size, const std::string& function, VMContext& context);    
        static void AssertStackSizeGreaterEqual(std::size_t size, const std::string& function, VMContext& context);    

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
            
            context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_NATIVE_PARAMETER_INVALID, function_name + ": argument type mismatch. Check specifications.");
            throw;
        }

        template<typename Type>
        static void ConvertStackedBigNumToString(const std::string& function_name, VMContext& context)
        {
            static_assert(std::is_same_v<Type, mp::mpf_float> || std::is_same_v<Type, mp::mpz_int>, "Type must be a bignum value!");
            ExecutorUtils::AssertStackSizeGreaterEqual(1, function_name, context);
            const auto& a = ExecutorUtils::TypeCheck<Type>(context.m_Stack.PopMove(), function_name, context);
            std::string result = a.str();
            context.m_Stack.Push(result);
        }
        
        template<typename Type>
        static void ConvertStackedStringToBigNum(const std::string& function_name, VMContext& context)
        {
            static_assert(std::is_same_v<Type, mp::mpf_float> || std::is_same_v<Type, mp::mpz_int>, "Type must be a bignum value!");
            ExecutorUtils::AssertStackSizeGreaterEqual(1, function_name, context);
            auto a = ExecutorUtils::TypeCheck<std::string>(context.m_Stack.PopMove(), function_name, context);
            try
            {
                Type result(a.c_str());
                context.m_Stack.Push(result);
            }
            catch(const std::exception& ex)
            {
                context.m_ErrorContext.LogCritical(RuntimeExceptionCodes::EX_RUNTIME_CONVERSION_ERROR, "Conversion error during '" + function_name + "' execution. String cannot be converted to a bignum.");
            }
        }

        template<typename SrcType, typename DstType>
        static void ConvertStackedBigNumToBigNum(const std::string& function_name, VMContext& context)
        {
            static_assert(std::is_same_v<SrcType, mp::mpf_float> || std::is_same_v<SrcType, mp::mpz_int>, "Type must be a bignum value!");
            static_assert(std::is_same_v<DstType, mp::mpf_float> || std::is_same_v<DstType, mp::mpz_int>, "Type must be a bignum value!");
            ExecutorUtils::AssertStackSizeGreaterEqual(1, function_name, context);
            auto a = ExecutorUtils::TypeCheck<SrcType>(context.m_Stack.PopMove(), function_name, context);
            DstType result = a.template convert_to<DstType>();
            context.m_Stack.Push(result);
        }
    };    
}
