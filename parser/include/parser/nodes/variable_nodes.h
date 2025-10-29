#pragma once

#include "base_node.h"

namespace parser
{
    class DeclareVariableNode : public ASTNode
    {
    public:
        DeclareVariableNode(std::string name, std::string type, std::unique_ptr<ASTNode> expression);
        DeclareVariableNode(std::string name, std::string type);

        void Compile(compiler::CompilerContext& context) override;
        
        bool HasExpression() const;
        
        const std::string GetName() const;
        const std::string GetTypeName() const;
        ASTNode& GetExpression() const;

    private:
        std::string m_Name;
        std::string m_TypeName;
        std::unique_ptr<ASTNode> m_Expression;
    };

    class AssignVariableNode : public ASTNode
    {
    public:
    
        AssignVariableNode(std::string name, std::unique_ptr<ASTNode> expression);

        void Compile(compiler::CompilerContext& context) override;

        ASTNode& GetExpression() const;
        std::string GetName() const;

    private:
        
        std::string m_Name;
        std::unique_ptr<ASTNode> m_Expression;
    };

    class AccessVariableNode : public ASTNode
    {
    public:
        AccessVariableNode(std::string name);

        void Compile(compiler::CompilerContext& context) override;

        std::string GetName() const;

    private:
        std::string m_Name;
    };
}
