#include "virtual-machine/environment.h"

#include <stdexcept>

#include <shared/objects/base_object.h>
#include <type_traits>

namespace vm
{
    Environment::Environment()
    {
        /** @todo reserve size. */
    }

    void Environment::AddScope()
    {
        m_Environments.emplace_back();
    }

    void Environment::RemoveScope()
    {
        if (m_Environments.empty())
        {
            return;
        }

        m_Environments.pop_back();
    }

    void Environment::AddVariable(std::size_t index, std::size_t scopeOffset, Types value)
    {
        if(scopeOffset >= m_Environments.size())
        {
            AddScope();
        }
        
        auto& scope = m_Environments.at(scopeOffset); /** @todo May throw an error, add vm-owned error handling. */

        if(index >= scope.size())
        {
            scope.emplace_back(std::move(value));
            return;
        }

        throw std::runtime_error("Variable confusion.");
    }

    Types& Environment::UnqualifiedLookup(std::size_t index, std::size_t scopeOffset)
    {
        if(scopeOffset >= m_Environments.size())
            throw std::runtime_error("Scope offset out of bounds for unqualified variable lookup.");

        auto& scope = m_Environments.at(scopeOffset);  /** @todo May throw an error, add vm-owned error handling. */

        if(index >= scope.size())
            throw std::runtime_error("Index is out of bounds for unqualified variable lookup.");

        return scope.at(index).m_Object; 
    }
}
