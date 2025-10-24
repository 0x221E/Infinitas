#pragma once

#include <string>
#include <cstdint>
#include <vector>
#include <unordered_map>
#include <expected>

namespace compiler
{
    enum class ResolutionError
    {
        ERROR_UNKNOWN = 0,
        ERROR_VAR_NOT_FOUND,
        ERROR_VAR_OUT_OF_SCOPE,
        ERROR_VAR_RE_DECL,
    };

    enum NameType : std::uint8_t
    {
        TYPE_VAR = 0,
        TYPE_FUNC = 1,
        TYPE_USERDEF = 2
    };

    struct ScopeInformation
    {
        std::size_t m_LastIndex;
    };

    struct SymbolEntry
    {
        std::size_t m_VariableIndex;
        std::size_t m_ScopeOffset;
        NameType m_Type;
    };

    class NameResolver
    {
    public:
        NameResolver();

        std::expected<SymbolEntry, ResolutionError> AddVariable(const std::string& name);
        std::expected<SymbolEntry, ResolutionError> ResolveUnqualifiedVariable(std::string name);

    public:
        void AddScope();
        void RemoveScope();

    private:
        std::vector<ScopeInformation> m_ScopeInformationSet;
        std::unordered_map<std::string, SymbolEntry> m_Symbols;
    };
}
