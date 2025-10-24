#include "parser/nodes/variable_nodes.h"

#include <compiler/compiler_context.h>

namespace parser
{
    DeclareVariableNode::DeclareVariableNode(std::string name, std::unique_ptr<ASTNode> expression)
        : m_Name(name), m_Expression(std::move(expression))
    {
    }

    DeclareVariableNode::DeclareVariableNode(std::string name)
        : m_Name(name), m_Expression(nullptr)
    {

    }


    void DeclareVariableNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileDeclareVariableNode(*this);
    }

    bool DeclareVariableNode::HasExpression() const
    {
        return m_Expression != nullptr;
    }

    std::string DeclareVariableNode::GetName() const
    {
        return m_Name;
    }

   ASTNode& DeclareVariableNode::GetExpression() const
    {
        return *m_Expression;
    }

    AssignVariableNode::AssignVariableNode(std::string name, std::unique_ptr<ASTNode> expression)
        : m_Name(name), m_Expression(std::move(expression))
    {
    }


    void AssignVariableNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileAssignVariableNode(*this);
    }

   ASTNode& AssignVariableNode::GetExpression() const
    {
        return *m_Expression;
    }
    
    std::string AssignVariableNode::GetName() const
    {
        return m_Name;
    }
    
    AccessVariableNode::AccessVariableNode(std::string name)
        : m_Name(name)
    {

    }

    void AccessVariableNode::Compile(compiler::CompilerContext &context)
    {
        context.CompileAccessVariableNode(*this);
    }

    std::string parser::AccessVariableNode::GetName() const
    {
        return m_Name;
    }
}
