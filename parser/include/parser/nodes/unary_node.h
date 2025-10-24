#pragma once

#include "base_node.h"

namespace parser
{
    class UnaryNode : public ASTNode
    {
    public:
        UnaryNode(shared::TokenType op, std::unique_ptr<ASTNode> expression);

        void Compile(compiler::CompilerContext& context) override;

        shared::TokenType GetOperator() const;
        ASTNode& GetExpression() const;

    private:
        shared::TokenType m_Operator;
        std::unique_ptr<ASTNode> m_Expression;
    };
}