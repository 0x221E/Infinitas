#include "virtual-machine/environment.h"

#include <shared/constant_pool.h>
#include <shared/object_visitors.h>
#include <stdexcept>
#include <shared/objects/base_object.h>

namespace vm
{
    Environment::Environment()
    {
        /** @todo reserve size. */
    }

    Environment::Environment(const Environment& other) : m_Environments(other.CloneStorage())
    {

    }
    
    Environment& Environment::operator=(const Environment& other)
    {
        if (this == &other) return *this;
        std::vector<StackValues> temp = other.CloneStorage();
        m_Environments.swap(temp);
        return *this;
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
    
    std::vector<StackValues> Environment::CloneStorage() const
    {
        std::vector<StackValues> temp;
        temp.reserve(m_Environments.size());
        for(const auto& env : m_Environments)
        {
            std::vector<EnvironmentValue> envTemp;
            envTemp.reserve(env.size());
            for(const auto& elem : env)
            {
                envTemp.emplace_back(std::visit(shared::CopyObject{}, elem.m_Object));
            }
            temp.emplace_back(std::move(envTemp));
        }
        return temp;
    }
}
