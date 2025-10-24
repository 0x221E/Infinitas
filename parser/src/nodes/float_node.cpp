#include "parser/nodes/float_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    FloatLiteralNode::FloatLiteralNode(mp::mpf_float value) : m_Value{value}
    {

    }
    
    void FloatLiteralNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileFloatNode(*this);
    }
    
    mp::mpf_float FloatLiteralNode::GetValue() const
    {
        return m_Value;
    }
}
