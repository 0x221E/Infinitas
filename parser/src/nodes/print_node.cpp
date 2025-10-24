#include "parser/nodes/function_call_node.h"

#include <compiler/compiler_context.h>

#include <parser/nodes/base_node.h>

namespace parser
{
    FunctionCallNode::FunctionCallNode(std::string name, std::vector<ASTNodePtr> arguments, bool isNative)
        : m_Name(name), m_Arguments(std::move(arguments)), m_IsNative(isNative)
    {
      LOG_DEBUG("Function call called.");
    }

    void FunctionCallNode::Compile(compiler::CompilerContext &context)
    {
      context.CompileFunctionCallNode(*this);
    }

    std::vector<ASTNodePtr>& FunctionCallNode::GetArguments()
    {
      return m_Arguments;
    }

    std::string FunctionCallNode::GetName() const
    {
      return m_Name;
    }
    
    bool FunctionCallNode::IsNative() const
    {
      return m_IsNative;
    }
}
