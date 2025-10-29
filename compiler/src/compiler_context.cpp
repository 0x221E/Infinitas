#include "compiler/compiler_context.h"

namespace compiler
{
    /**
     * @todo This will be heavily optimized, and organized.
     */
    const std::unordered_map<std::string, shared::Opcodes> s_Functions = 
    {
        {"print", shared::Opcodes::OP_NATIVE_PRINT},
        {"exit", shared::Opcodes::OP_NATIVE_EXIT},
        {"integer_add", shared::Opcodes::OP_NATIVE_INT_ADD},
        {"float_add", shared::Opcodes::OP_NATIVE_FLOAT_ADD},
        {"integer_sub", shared::Opcodes::OP_NATIVE_INT_SUB},
        {"float_sub", shared::Opcodes::OP_NATIVE_FLOAT_SUB},
        {"integer_mul", shared::Opcodes::OP_NATIVE_INT_MUL},
        {"float_mul", shared::Opcodes::OP_NATIVE_FLOAT_MUL},
        {"integer_div", shared::Opcodes::OP_NATIVE_INT_DIV},
        {"float_div", shared::Opcodes::OP_NATIVE_FLOAT_DIV},
        {"integer_to_string", shared::Opcodes::OP_NATIVE_INT_TO_STR},
        {"integer_to_float", shared::Opcodes::OP_NATIVE_INT_TO_FLOAT},
        {"float_to_string", shared::Opcodes::OP_NATIVE_FLOAT_TO_STR},
        {"float_to_integer", shared::Opcodes::OP_NATIVE_FLOAT_TO_INT},
        {"string_to_float", shared::Opcodes::OP_NATIVE_STR_TO_FLOAT},
        {"string_to_integer", shared::Opcodes::OP_NATIVE_STR_TO_INT},
        {"negate_integer", shared::Opcodes::OP_NATIVE_UNARY_NEGATE_INT},
        {"negate_float", shared::Opcodes::OP_NATIVE_UNARY_NEGATE_FLOAT},
        {"integer_to_string", shared::Opcodes::OP_NATIVE_INT_TO_STR}
    };

    CompilerContext::CompilerContext()
    {
        
    }

    void CompilerContext::CompileBinaryNode(parser::BinaryOperation &node)
    {
        node.GetLeft().Compile(*this);
        node.GetRight().Compile(*this);

        switch (node.GetOperator())
        {
        case shared::TokenType::TOKEN_PLUS:
        {
            break;
        }

        case shared::TokenType::TOKEN_MINUS:
        {
            break;
        }

        case shared::TokenType::TOKEN_ASTERISK:
        {
            break;
        }

        case shared::TokenType::TOKEN_SLASH:
        {
            break;
        }

        default:
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_OP_NOT_FOUND, "Operator not found during binary operation compilation."); 
        }
        }
    }

    void CompilerContext::CompileIntegerNode(parser::IntegerLiteralNode& node)
    {
        m_Constants.InsertConstant(node.GetValue());
        node.SetRuntimeType(parser::NodeRuntimeType::INTEGER);
        m_Instructions.emplace_back(shared::Opcodes::OP_LOAD_CONSTANT, m_Constants.Size() - 1);
    }

    void CompilerContext::CompileFloatNode(parser::FloatLiteralNode& node)
    {
        m_Constants.InsertConstant(node.GetValue());
        node.SetRuntimeType(parser::NodeRuntimeType::FLOAT) ;
        m_Instructions.emplace_back(shared::Opcodes::OP_LOAD_CONSTANT, m_Constants.Size() - 1);
    }

    void CompilerContext::CompileStringNode(parser::StringLiteralNode& node)
    {
        m_Constants.InsertConstant(node.GetValue());
        node.SetRuntimeType(parser::NodeRuntimeType::STRING);
        m_Instructions.emplace_back(shared::Opcodes::OP_LOAD_CONSTANT, m_Constants.Size() - 1);
    }

    void CompilerContext::CompileUnaryNode(parser::UnaryNode& node)
    {
        node.GetExpression().Compile(*this);
        node.SetRuntimeType(node.GetExpression().GetRuntimeType());

        switch (node.GetOperator())
        {
        case shared::TokenType::TOKEN_MINUS:
        {
            m_Instructions.emplace_back(shared::Opcodes::OP_UNARY_NEGATION);
            break;
        }
        default:
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_OP_NOT_FOUND, "Operator not found during unary operation compilation."); 
        }
        }
    }

    void CompilerContext::CompileGroupingNode(parser::GroupingExpression &node)
    {
        node.GetExpression().Compile(*this);
        node.SetRuntimeType(node.GetExpression().GetRuntimeType());
    }

    /**
     * @todo Specify this behavior!
     * @warning Temporary behavior that only handles native functions! Implementation will change and this function is not optimal architecturally!
     */
    void CompilerContext::CompileFunctionCallNode(parser::FunctionCallNode &node)
    {
        auto& arguments = node.GetArguments();
        for(auto& argument : arguments)
        {
            argument->Compile(*this);
        }

        if(node.IsNative() == false)
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_UNKNOWN, "User-defined function calls do not exist right now. See specifications.");
            return;
        }
      
        auto result = s_Functions.find(node.GetName());
        if(result == s_Functions.end())
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_OP_NOT_FOUND, "Native function '" + node.GetName() + "' not found.");
            return;
        }
        
        m_Instructions.emplace_back(result->second, node.GetArguments().size());
    }

    void CompilerContext::CompileBlockNode(parser::BlockNode &node)
    {
        m_Instructions.emplace_back(shared::Opcodes::OP_BLOCK_START);
        m_Resolver.AddScope();

        for (auto &&statement : node.GetStatements())
        {
          statement->Compile(*this);
        }

        m_Instructions.emplace_back(shared::Opcodes::OP_BLOCK_END);
        m_Resolver.RemoveScope();
    }

    void CompilerContext::CompileErrorNode(parser::ErrorNode &node)
    {
        m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_UNKNOWN, "It seems like parsing errors continued into compilation. This should not have happened. This is likely a bug inside Infinitas. Please let developers know."); 
    }

    void CompilerContext::CompileDeclareVariableNode(parser::DeclareVariableNode &node)
    {
        if (!node.HasExpression())
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_COMPILER_VAR_IMPROPER,"Variables must be declared with an initially valid value."); 
        }

        node.GetExpression().Compile(*this);

        auto result = m_Resolver.AddVariable(node.GetName());

        if (result.error() == ResolutionError::ERROR_VAR_RE_DECL)
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_RESOLUTION_VAR_RE_DECL,"Variable resolution error. Variable is already declared."); 
            return;
        }

        if(result.value().m_Type != NameType::TYPE_VAR)
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_RESOLUTION_SYMBOL_ERR, "Symbol resolution error. Variable was expected."); 
            return;
        }

        m_Instructions.emplace_back(shared::Opcodes::OP_DECLARE_VARIABLE, result.value().m_VariableIndex, result.value().m_ScopeOffset);
    }

    void CompilerContext::CompileAssignVariableNode(parser::AssignVariableNode &node)
    {
        auto result = m_Resolver.ResolveUnqualifiedVariable(node.GetName());

        if(!result || result.value().m_Type != NameType::TYPE_VAR)
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_RESOLUTION_SYMBOL_ERR, "Variable '" + node.GetName() + "' does not exist.");
            return;
        }

        node.GetExpression().Compile(*this); 

        m_Instructions.emplace_back(shared::Opcodes::OP_ASSIGN_VARIABLE, result.value().m_VariableIndex, result.value().m_ScopeOffset);
    }

    void CompilerContext::CompileAccessVariableNode(parser::AccessVariableNode &node)
    {
        auto result = m_Resolver.ResolveUnqualifiedVariable(node.GetName());

        if(!result)
        {
            m_ErrorContext.LogCritical(CompilerExceptionCodes::EX_RESOLUTION_VAR_NOT_FOUND, "Variable not found: " + node.GetName());
            return;
        }

        m_Instructions.emplace_back(shared::Opcodes::OP_ACCESS_VARIABLE, result.value().m_VariableIndex, result.value().m_ScopeOffset);
    }

    CompilerResult CompilerContext::GetResult()
    {
        return {std::move(m_Instructions), std::move(m_Constants)};
    }

    bool CompilerContext::ErrorThrown() const
    {
        return m_ErrorContext.m_ErrorThrown;
    }

    void CompilerContext::ResetErrorState()
    {
        m_ErrorContext.m_ErrorThrown = false;
    }

    void CompilerContext::PassConstants(shared::ConstantPool&& constants)
    {
        m_Constants = std::move(constants);
    }
}
