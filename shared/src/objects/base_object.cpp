#include "shared/objects/base_object.h"

namespace shared 
{
    
    Object::Object(ObjectType type) : m_Type{type}
    {

    }

    ObjectType Object::GetType() const
    {
        return m_Type;
    }
}