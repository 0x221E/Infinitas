#include "compiler/compiler.h"

#include "compiler/compiler_context.h"

#include <shared/logger.h>

namespace compiler
{
    Compiler::Compiler()
    {
        
    }

    void Compiler::PassConstants(shared::ConstantPool&& constants)
    {
        m_Context.PassConstants(std::move(constants));
    }

    void Compiler::ResetErrors() noexcept
    {
        m_Context.ResetErrorState();
    }

    std::expected<CompilerResult, bool> Compiler::Compile(std::vector<std::unique_ptr<parser::ASTNode>> &&tree)
    {
        try
        {
            for (auto &statement : tree)
            {
                statement->Compile(m_Context);
            }
        }
        catch(const CompilerException& err)
        {
            return std::unexpected(true); 
        }
        
        if(m_Context.ErrorThrown())
        {
          return std::unexpected(true);
        }

        CompilerResult result = m_Context.GetResult();
        return result;
    }
}
