#include "parser/nodes/integer_node.h"

#include <compiler/compiler_context.h>


namespace parser
{
    IntegerLiteralNode::IntegerLiteralNode(mp::mpz_int value) : m_Value{value}
    {

    }

    void IntegerLiteralNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileIntegerNode(*this);
    }
    
    mp::mpz_int IntegerLiteralNode::GetValue() const
    {
        return m_Value;
    }
}
