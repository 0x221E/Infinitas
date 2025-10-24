#pragma once

#include <expected>
#include <variant>
#include <shared/objects/base_object.h>

namespace vm
{
    /**
     * @brief A utility function that checks the type against the supplied template argument.
     * @returns If matches the supplied type, returns the object as the type specified in template arguments, otherwise a boolean indicating error.
     */
    template<typename ExpectedType, typename T>
    std::expected<ExpectedType, bool> TypeCheck(T&& obj)
    {
        auto* value = std::get_if<ExpectedType>(&obj);
        if (value)
        {
            return *value;
        }
        else
        {
            return std::unexpected<bool>(true);
        }
    }
}
