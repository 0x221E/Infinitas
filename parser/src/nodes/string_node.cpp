#include "parser/nodes/string_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    StringLiteralNode::StringLiteralNode(std::string value) : m_Value{value}
    {
    }

    void StringLiteralNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileStringNode(*this);
    }

    std::string parser::StringLiteralNode::GetValue() const
    {
        return m_Value;
    }
}
