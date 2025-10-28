#pragma once

#include "../virtual_machine.h"

namespace vm
{
    namespace typeConversion
    {
        /**
         * @brief Converts the top value on the stack to a string. 
         *
         * @details This function implements conversion from the Virtual Machine's integer type
         * to its string representation.
         * @spec 4.3.2.3
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be an integer, or type mismatch error is thrown.
         * @note The integer value is removed from stack, and the string result is pushed.
         */
        void IntegerToString(VMContext& context, const shared::Instruction& instruction);

        /**
         * @brief Converts the top value on the stack to a float. 
         *
         * @details This function implements conversion from the Virtual Machine's integer type
         * to its floating-point representation.
         * @spec 4.3.2.2
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be a integer, or type mismatch error is thrown.
         * @note The integer value is removed from stack, and the float result is pushed.
         */
        void IntegerToFloat(VMContext& context, const shared::Instruction& instruction);

        /**
         * @brief Converts the top value on the stack to a string. 
         *
         * @details This function implements conversion from the Virtual Machine's floating-point type
         * to its string representation.
         * @spec 4.3.2.4
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be a float, or type mismatch error is thrown.
         * @note The float value is removed from stack, and the string result is pushed.
         */
        void FloatToString(VMContext& context, const shared::Instruction& instruction);

        /**
         * @brief Converts the top value on the stack to a integer. 
         *
         * @details This function implements conversion from the Virtual Machine's floating-point type
         * to its integer representation.
         * @spec 4.3.2.1
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be a float, or type mismatch error is thrown.
         * @note The float value is removed from stack, and the integer result is pushed.
         */
        void FloatToInteger(VMContext& context, const shared::Instruction& instruction);

        /**
         * @brief Converts the top value on the stack to a float. 
         *
         * @details This function implements conversion from the Virtual Machine's string representation to
         * to its floating-point representation.
         * @spec 4.3.2.5
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be a string, or type mismatch error is thrown.
         * @note The string value is removed from stack, and the float result is pushed.
         */
        void StringToFloat(VMContext& context, const shared::Instruction& instruction);

        /**
         * @brief Converts the top value on the stack to a integer. 
         *
         * @details This function implements conversion from the Virtual Machine's string representation to
         * to its integer representation.
         * @spec 4.3.2.6
         *
         * @param context The Virtual Machine cpontext that is used to access the Virtual Machine internals.
         * @param instruction The instruction that is dispatched (current instruction). Optional.
         *
         * @pre Top value on the stack has to be a string, or type mismatch error is thrown.
         * @note The string value is removed from stack, and the integer result is pushed.
         */
        void StringToInteger(VMContext& context, const shared::Instruction& instruction);
    }
}
