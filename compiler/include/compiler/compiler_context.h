#pragma once

#include <parser/nodes/nodes.h>
#include <shared/instruction_container.h>
#include <shared/constant_pool.h>
#include <shared/objects/all.h>

#include "compiler/exception_compiler.h"
#include "name_resolver.h"
#include <parser/nodes/function_call_node.h>
#include <shared/error_context.h>

namespace compiler
{
    extern const std::unordered_map<std::string, shared::Opcodes> s_Functions;

    struct CompilerResult
    {
        std::vector<shared::Instruction> m_Instructions;
        shared::ConstantPool m_Constants;
    };

    class CompilerContext
    {
    public:
        CompilerContext();

    public:
        void CompileBinaryNode(parser::BinaryOperation& node);
        
        void CompileIntegerNode(parser::IntegerLiteralNode& node);
        void CompileFloatNode(parser::FloatLiteralNode& node);
        void CompileStringNode(parser::StringLiteralNode& node);
        
        void CompileUnaryNode(parser::UnaryNode& node);
        void CompileGroupingNode(parser::GroupingExpression& node);

        void CompileFunctionCallNode(parser::FunctionCallNode& node);

        void CompileBlockNode(parser::BlockNode& node);
        
        void CompileErrorNode(parser::ErrorNode& node);

        void CompileDeclareVariableNode(parser::DeclareVariableNode& node);
        void CompileAssignVariableNode(parser::AssignVariableNode& node);
        void CompileAccessVariableNode(parser::AccessVariableNode& node);
    
    public:
        CompilerResult GetResult();
        bool ErrorThrown() const;
        void ResetErrorState();
        void PassConstants(shared::ConstantPool&& constants);

    private:
        std::vector<shared::Instruction> m_Instructions;
        shared::ConstantPool m_Constants;
        NameResolver m_Resolver;
        shared::ErrorContext<CompilerException> m_ErrorContext;
    };
}
