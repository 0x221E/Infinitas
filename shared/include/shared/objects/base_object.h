/**
 * @file objects.hpp
 * @author 0x221E
 */

#pragma once

#include <boost/multiprecision/gmp.hpp>
#include <cstdint>
#include <iostream>

#include "../useful.h"

namespace shared
{

    enum ObjectType : uint8_t
    {
        OBJ_INVALID = 0x00,
        OBJ_INT = 0x01,
        OBJ_FLOAT = 0x02,
        OBJ_STRING = 0x03
    };
    
    class Object
    {
    public:
    /** Constructor/Destructor */
    
        Object(ObjectType type);

        virtual ~Object() = default;

    public:
    /** Virtual functions */
        ObjectType GetType() const;

        virtual ObjectPtr Clone() const = 0;

    private:
    /** Private variables */

        ObjectType m_Type;
    };

}
