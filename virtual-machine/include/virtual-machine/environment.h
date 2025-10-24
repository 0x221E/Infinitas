#pragma once

#include <vector>
#include <unordered_map>

#include <shared/useful.h>

namespace vm
{
    struct EnvironmentValue
    {
        Types m_Object;
    };

    using StackValues = std::vector<EnvironmentValue>;

    class Environment
    {
    public:
        Environment();
    
    public:
        void AddScope();
        void RemoveScope();

        void AddVariable(std::size_t index, std::size_t scopeOffset, Types value);

        Types& UnqualifiedLookup(std::size_t index, std::size_t scopeOffset);

    public:
        std::vector<StackValues> m_Environments;
    };
}
