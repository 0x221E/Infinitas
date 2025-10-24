#pragma once

#include "base_node.h"

namespace parser
{

    class FloatLiteralNode : public ASTNode
    {
    public:
    /** Constructors */
    
        FloatLiteralNode(mp::mpf_float value);

    public:
        void Compile(compiler::CompilerContext& context) override;

    public:
    /** Public accessors */
        
        mp::mpf_float GetValue() const;

    private:
    /** Private variables */
    
        mp::mpf_float m_Value;
    };
}