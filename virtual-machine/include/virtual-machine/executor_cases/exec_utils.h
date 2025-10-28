#pragma once

#include <type_traits>
#include <variant>
#include <shared/objects/base_object.h>
#include "virtual-machine/exception_runtime.h"
#include "virtual-machine/virtual_machine.h"

namespace vm
{
    /**
     * @brief Holds utility functions for the executor cases of the virtual machine.
     * @details This class aids in runtime functionality, and used to reduce redundant code related to generic utilities.
     */
    class ExecutorUtils
    {
    public:
        /**
         * @brief Throws an error if the total size of stack does not meet the supplied size.
         *
         * @param size Requested stack size.
         * @param function Function name to be displayed on error messages.
         * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
         *
         * @warning Should be used carefully since this function will check for whether the stack is equal, and that may change
         * * depending on whether the values that resulted from other operations still remain in the stack.
         */
        static void AssertStackSizeNotEqual(std::size_t size, const std::string& function, VMContext& context);    

        /**
         * @brief Throws an error if the size of stack is less than the supplied size.
         *
         * @param size Requested stack size.
         * @param function Function name to be displayed on error messages.
         * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
         */
        static void AssertStackSizeGreaterEqual(std::size_t size, const std::string& function, VMContext& context);    

    public:
       /**
       * @brief A utility function that checks the type against the supplied template argument.
       * @returns If matches the supplied type, returns the object as the type specified in template arguments, otherwise a runtime error is thrown.
       *
       * @param obj Object to be checked.
       * @param function Function name to be displayed on error messages.
       * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
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

        /**
         * @brief Converts the big number on top of the stack to a string.
         *
         * @param function_name Name of the function to be displayed on error messages.
         * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
         *
         * @tparam Type The big number type to be converted to string.
         * 
         * @pre The template parameter must be either a mp::mpf_float or mp::mpz_int, otherwise a static error is thrown.
         * @note The big number on the stack is popped and moved, and then resulting string is pushed.
         */
        template<typename Type>
        static void ConvertStackedBigNumToString(const std::string& function_name, VMContext& context)
        {
            static_assert(std::is_same_v<Type, mp::mpf_float> || std::is_same_v<Type, mp::mpz_int>, "Type must be a bignum value!");
            ExecutorUtils::AssertStackSizeGreaterEqual(1, function_name, context);
            const auto& a = ExecutorUtils::TypeCheck<Type>(context.m_Stack.PopMove(), function_name, context);
            std::string result = a.str();
            context.m_Stack.Push(result);
        }

        /**
         * @brief Converts the string on top of the stack to a big number.
         *
         * @param function_name Name of the function to be displayed on error messages.
         * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
         *
         * @tparam Type The big number type to be constructed from string.
         * 
         * @pre The template parameter must be either a mp::mpf_float or mp::mpz_int, otherwise a static error is thrown.
         * @note The string on the stack is popped and moved, and then resulting big number is pushed.
         *
         * @warning If the big number cannot be constructed due to an invalid string, a runtime conversion error will be thrown.
         */
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

        /**
         * @brief Converts the big number on top of the stack to another big number.
         *
         * @param function_name Name of the function to be displayed on error messages.
         * @param context The Virtual Machine context struct that holds references to the Virtual Machine internals.
         *
         * @tparam SrcType The big number type to be converted to another big number.
         * @tparam DstType The destionation big number type.
         * 
         * @pre The template parameters must be either a mp::mpf_float or mp::mpz_int, otherwise a static error is thrown.
         * @note The big number on the stack is popped and moved, and then resulting big number is pushed.
         */
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
