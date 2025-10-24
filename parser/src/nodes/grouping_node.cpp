#include "parser/nodes/grouping_node.h"

#include <compiler/compiler_context.h>

namespace parser
{
    GroupingExpression::GroupingExpression(std::unique_ptr<ASTNode> expression) : m_Expression(std::move(expression))
    {
    }

    void GroupingExpression::Compile(compiler::CompilerContext &context)
    {
        context.CompileGroupingNode(*this);
    }
    
    ASTNode& GroupingExpression::GetExpression() const
    {
        return *m_Expression;
    }

}
