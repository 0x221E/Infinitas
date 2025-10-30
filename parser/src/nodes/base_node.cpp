#include "parser/nodes/base_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    ASTNode::ASTNode()
    {
        
    }
    
    void ASTNode::SetRuntimeType(NodeRuntimeType type)
    {
        m_RuntimeType = type;
    }

    const NodeRuntimeType ASTNode::GetRuntimeType() const
    {
        return m_RuntimeType;
    }

    const NodeType ASTNode::GetNodeType() const
    {
	return m_Type;
    }
}
