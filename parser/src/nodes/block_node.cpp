#include "parser/nodes/block_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    BlockNode::BlockNode()
    {

    }
    
    BlockNode::BlockNode(std::vector<std::unique_ptr<ASTNode>>&& statements) : m_Statements(std::move(statements))
    {

    }

    void BlockNode::Compile(compiler::CompilerContext& context)
    {
        context.CompileBlockNode(*this);
    }

    std::vector<std::unique_ptr<ASTNode>>& BlockNode::GetStatements()
    {
        return m_Statements;
    }
}
