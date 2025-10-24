#pragma once

#include "base_node.h"

namespace parser
{

    class FunctionCallNode : public ASTNode
    {
    public:
        FunctionCallNode(std::string name, std::vector<ASTNodePtr> arguments, bool isNative);
        
    public:
        void Compile(compiler::CompilerContext& context) override;

    public:
        std::vector<ASTNodePtr>& GetArguments();
        std::string GetName() const;
        bool IsNative() const;

    private:
        std::string m_Name;
        std::vector<ASTNodePtr> m_Arguments;
        bool m_IsNative;
    };
}
