#pragma once

#include <cstdint>
#include <iostream>
#include <memory>

#include <shared/token.h>

#include <boost/multiprecision/gmp.hpp>

namespace mp = boost::multiprecision;

namespace compiler{
    class CompilerContext;
}

namespace parser
{
    enum NodeType : uint8_t
    {
        NODE_ERROR = 0x00,
        NODE_BINARY = 0x01,
        NODE_INTLITERAL = 0x02,
        NODE_FLOATLITERAL = 0x03,
        NODE_STRINGLITERAL = 0x04,
        NODE_DECLAREVARIABLE = 0x05,
        NODE_ASSIGNVARIABLE = 0x06,
        NODE_ACCESSVARIABLE = 0x07,
        NODE_GROUPING = 0x08,
        NODE_UNARY = 0x09,
        NODE_PRINT = 0x10,
        NODE_EXIT = 0x11,
        NODE_BLOCK = 0x12
    };
    
    enum class NodeRuntimeType : std::uint8_t
    {
        UNKNOWN = 0,
        INTEGER = 1,
        FLOAT = 2,
        STRING = 3
    };
    
    class ASTNode
    {
    public:
        ASTNode();

        virtual ~ASTNode() = default;
    
    public:
        virtual void Compile(compiler::CompilerContext& context) = 0;
    
    public:
        void SetRuntimeType(NodeRuntimeType type);
        const NodeRuntimeType GetRuntimeType() const;

    private:
        NodeType m_Type;
        NodeRuntimeType m_RuntimeType;
    };

    using ASTNodePtr = std::unique_ptr<ASTNode>;
}
