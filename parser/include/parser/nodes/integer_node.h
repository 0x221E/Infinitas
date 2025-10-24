#pragma once

#include "base_node.h"

namespace parser
{
    class IntegerLiteralNode : public ASTNode
    {
    public:
    /** Constructors */

        IntegerLiteralNode(mp::mpz_int value);

    public:
        void Compile(compiler::CompilerContext& context) override;

    public:
    /** Public accessors */
        mp::mpz_int GetValue() const;

    private:
    /** Private variables */

        mp::mpz_int m_Value;
    };
}