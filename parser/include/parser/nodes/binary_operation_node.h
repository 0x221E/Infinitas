#pragma once

#include "base_node.h"

namespace parser
{
    class BinaryOperation : public ASTNode
    {
    public:
        BinaryOperation(shared::TokenType operation, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right);

    public:
        ASTNode &GetLeft();
        ASTNode &GetRight();

        shared::TokenType GetOperator() const;

        void Compile(compiler::CompilerContext& context) override;

    private:
        std::unique_ptr<ASTNode> m_Left, m_Right;
        shared::TokenType m_Operator;
    };
}