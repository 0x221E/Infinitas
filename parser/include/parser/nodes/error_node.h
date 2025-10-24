#pragma once

#include "base_node.h"

namespace parser
{
    class ErrorNode : public ASTNode
    {
    public:
        ErrorNode();

        void Compile(compiler::CompilerContext& context) override;
    };
}