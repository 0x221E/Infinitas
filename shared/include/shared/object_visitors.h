#pragma once

#include "shared/objects/base_object.h"
#include "shared/useful.h"

namespace shared
{
    class CopyObject
    {
    public:
        Types operator()(const ObjectPtr& object) const
        {
            return object->Clone();
        }

        template<typename T>
        Types operator()(const T& all) const
        {
            return all;
        }
    };
}
