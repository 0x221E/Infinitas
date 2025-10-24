#pragma once

#include <parser/nodes/base_node.h>
#include "compiler_context.h"
#include <shared/constant_pool.h>

#include <vector>
#include <memory>

namespace compiler{

    /**
     * @todo [FUTURE] Add concurrent-compilation support.
     */
    class Compiler 
    {
    public:
        Compiler();

    public:
        void PassConstants(shared::ConstantPool&& constants);
        void ResetErrors() noexcept;
        std::expected<CompilerResult, bool> Compile(std::vector<std::unique_ptr<parser::ASTNode>>&& tree);

    private:
        CompilerContext m_Context;
    };

}
