#pragma once

#include "base_node.h"

namespace parser
{
    class BlockNode : public ASTNode
    {
    public:
        BlockNode();
        BlockNode(std::vector<std::unique_ptr<ASTNode>>&& statements);

    public:
        void Compile(compiler::CompilerContext& context) override;    

    public:
        std::vector<std::unique_ptr<ASTNode>>& GetStatements();

    private:
        std::vector<std::unique_ptr<ASTNode>> m_Statements;
    };
}