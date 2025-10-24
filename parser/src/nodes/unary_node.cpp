#include "parser/nodes/unary_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    UnaryNode::UnaryNode(shared::TokenType op, std::unique_ptr<ASTNode> expression)
        : m_Operator(op), m_Expression(std::move(expression))
    {
    }

    void UnaryNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileUnaryNode(*this);
    }

    shared::TokenType UnaryNode::GetOperator() const
    {
        return m_Operator;
    }

    ASTNode &UnaryNode::GetExpression() const
    {
        return *m_Expression;
    }
}
