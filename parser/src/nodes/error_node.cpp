#include "parser/nodes/error_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    ErrorNode::ErrorNode()
    {
    }

    void ErrorNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileErrorNode(*this);
    }
}
