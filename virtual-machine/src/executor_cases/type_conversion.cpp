#include "virtual-machine/executor_cases/type_conversion.h"

#include "virtual-machine/executor_cases/exec_utils.h"

namespace vm
{

    void typeConversion::IntegerToString(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedBigNumToString<mp::mpz_int>("__integer_to_string", context);
    }

    void typeConversion::IntegerToFloat(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedBigNumToBigNum<mp::mpz_int,mp::mpf_float>("__integer_to_float", context);
    }

    void typeConversion::FloatToString(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedBigNumToString<mp::mpf_float>("__float_to_string", context);
    }
    
    void typeConversion::FloatToInteger(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedBigNumToBigNum<mp::mpf_float, mp::mpz_int>("__float_to_integer", context);
    }

    void typeConversion::StringToFloat(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedStringToBigNum<mp::mpf_float>("__string_to_float", context);
    }

    void typeConversion::StringToInteger(VMContext& context, const shared::Instruction& instruction)
    {
        ExecutorUtils::ConvertStackedStringToBigNum<mp::mpz_int>("__string_to_integer", context);
    }
}
