#pragma once

#include "base_node.h"

namespace parser
{
    class StringLiteralNode : public ASTNode
    {
    public:
        StringLiteralNode(std::string value);
    
    public:
        void Compile(compiler::CompilerContext& context) override;

        std::string GetValue() const;
    
    private:
        std::string m_Value;
    };
}