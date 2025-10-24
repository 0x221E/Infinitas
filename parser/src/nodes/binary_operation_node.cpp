#include "parser/nodes/binary_operation_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    BinaryOperation::BinaryOperation(shared::TokenType operation, std::unique_ptr<ASTNode> left, std::unique_ptr<ASTNode> right)
        : m_Operator(operation), m_Left(std::move(left)), m_Right(std::move(right))
    {
    }

    ASTNode &BinaryOperation::GetLeft()
    {
        return *m_Left;
    }

    ASTNode &BinaryOperation::GetRight()
    {
        return *m_Right;
    }

    shared::TokenType BinaryOperation::GetOperator() const
    {
        return m_Operator;
    }

    void BinaryOperation::Compile(compiler::CompilerContext &context)
    {
        context.CompileBinaryNode(*this);
    }
}
