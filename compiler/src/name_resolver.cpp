#include "compiler/name_resolver.h"

#include <stdexcept>

namespace compiler
{
    NameResolver::NameResolver()
    {
        
    }

    std::expected<SymbolEntry, ResolutionError> NameResolver::AddVariable(const std::string &name)
    {
        if (m_ScopeInformationSet.size() == 0)
        {
            AddScope();
        }

        auto search = ResolveUnqualifiedVariable(name);

        if(search.has_value()){
            return std::unexpected(ResolutionError::ERROR_VAR_RE_DECL);
        }

        auto& currentScope = m_ScopeInformationSet.back();

        SymbolEntry entry{currentScope.m_LastIndex, m_ScopeInformationSet.size() - 1, NameType::TYPE_VAR};
        m_Symbols.emplace(name, entry);
        currentScope.m_LastIndex++;

        return entry;
    }

    std::expected<SymbolEntry, ResolutionError> NameResolver::ResolveUnqualifiedVariable(std::string name)
    {
        auto search = m_Symbols.find(name);

        if (search == m_Symbols.end())
        {
            return std::unexpected(ResolutionError::ERROR_VAR_NOT_FOUND);
        }

        if (search->second.m_ScopeOffset >= m_ScopeInformationSet.size())
        {
            return std::unexpected(ResolutionError::ERROR_VAR_OUT_OF_SCOPE);
        }

        return search->second;
    }

    void NameResolver::AddScope()
    {
        m_ScopeInformationSet.emplace_back(0);
    }

    void NameResolver::RemoveScope()
    {
        m_ScopeInformationSet.pop_back();
    }
}
