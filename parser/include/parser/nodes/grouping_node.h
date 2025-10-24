#pragma once

#include "base_node.h"

namespace parser
{
    class GroupingExpression : public ASTNode
    {
    public:
        GroupingExpression(std::unique_ptr<ASTNode> expression);
    
    public:
        void Compile(compiler::CompilerContext& context) override;

    public:
        ASTNode& GetExpression() const;

    private:
        std::unique_ptr<ASTNode> m_Expression;
    };
}