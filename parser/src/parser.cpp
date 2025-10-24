#include "parser/parser.h"
#include "parser/exception_syntax.h"
#include "parser/nodes/base_node.h"

namespace parser
{

    Parser::Parser() : m_Tokens{}, m_CurrentPosition{0}, m_ErrorContext{}
    {
    }

    std::expected<std::vector<ASTNodePtr>, bool> Parser::Parse(std::vector<shared::Token> tokens)
    {
        m_Tokens = tokens;
        Cleanup();
        std::vector<ASTNodePtr> statements;
        statements.reserve(tokens.size()); /** @todo Temporary reserve(), implement a smart detection logic since token size do not reflect statement count accurately. */
     
        while (!IsAtEnd())
        {
            statements.emplace_back(Declaration());
        }
      
        if(m_ErrorContext.m_ErrorThrown) 
/** We want to check for panic state AFTER the parsing is complete to have as much errors as possible on a single run (which is very desirable). */
        {
          return std::unexpected(true);
        }

        return statements;
    }

    void Parser::Cleanup()
    {
        m_CurrentPosition = 0;
        m_ErrorContext.m_ErrorThrown = false;
    }

    shared::Token Parser::Peek()
    {
        return m_Tokens.at(m_CurrentPosition);
    }

    shared::Token Parser::Previous()
    {
        return m_Tokens.at(m_CurrentPosition - 1);
    }

    bool Parser::IsAtEnd()
    {
        return Peek().type == shared::TokenType::TOKEN_END;
    }

    shared::Token Parser::Advance()
    {
        if (!IsAtEnd())
            m_CurrentPosition++;
        return Previous();
    }

    bool Parser::Check(shared::TokenType type)
    {
        if (IsAtEnd())
            return false;
        return Peek().type == type;
    }

    void Parser::Expect(shared::TokenType expectedToken, SyntaxExceptionCodes errorCode, std::string errorMessage)
    {
        if (Check(expectedToken))
        {
            Advance();
            return;
        }

        m_ErrorContext.LogCritical(errorCode, errorMessage);
    }

    shared::Token Parser::Consume(shared::TokenType expectedToken, SyntaxExceptionCodes errorCode, std::string errorMessage)
    {
        if (Check(expectedToken))
        {
            return Advance();
        }

        m_ErrorContext.LogCritical(errorCode, errorMessage);
        return shared::Token();
    }

    ASTNodePtr Parser::Primary()
    {
        if (Match(shared::TokenType::TOKEN_INTEGER_LITERAL))
        {
            return std::make_unique<IntegerLiteralNode>(mp::mpz_int(Previous().lexeme));
        }

        if (Match(shared::TokenType::TOKEN_FLOAT_LITERAL))
        {
            return std::make_unique<FloatLiteralNode>(mp::mpf_float(Previous().lexeme));
        }

        if (Match(shared::TokenType::TOKEN_STRING_LITERAL))
        {
            return std::make_unique<StringLiteralNode>(Previous().lexeme);
        }

        if (Match(shared::TokenType::TOKEN_NATIVE_FUNCTION))
        {
            std::string name = Previous().lexeme;

            if(Peek().type == shared::TokenType::TOKEN_LEFT_PARENTHESIS)
            {
                return FunctionCall(name, true);
            }
        }

        if (Match(shared::TokenType::TOKEN_IDENTIFIER))
        {
            std::string name = Previous().lexeme;

            if(Peek().type == shared::TokenType::TOKEN_LEFT_PARENTHESIS)
            {
                return FunctionCall(name, false);
            }

            return std::make_unique<AccessVariableNode>(name);
        }

        if (Match(shared::TokenType::TOKEN_LEFT_PARENTHESIS))
        {
            ASTNodePtr expression = Expression();
            Expect(shared::TokenType::TOKEN_RIGHT_PARENTHESIS, SyntaxExceptionCodes::EX_SYNTAX_MISSING_RIGHTPAREN,
                   "Closing paranthesis ')' is missing on grouping expression.");
            return std::make_unique<GroupingExpression>(std::move(expression));
        }

        m_ErrorContext.LogCritical(SyntaxExceptionCodes::EX_SYNTAX_UNKNOWN, 
                                     "Parser ran out of path.");
        return std::make_unique<ErrorNode>();
    }

    ASTNodePtr Parser::Unary()
    {
        if (Match(shared::TokenType::TOKEN_MINUS))
        {
            shared::TokenType op = Previous().type;
            ASTNodePtr right = Unary();
            return std::make_unique<UnaryNode>(op, std::move(right));
        }

        return Primary();
    }

    ASTNodePtr Parser::Factor()
    {
        ASTNodePtr expression = Unary();

        while (Match(shared::TokenType::TOKEN_ASTERISK, shared::TokenType::TOKEN_SLASH))
        {
            shared::TokenType operation = Previous().type;
            ASTNodePtr right = Unary();
            expression = std::make_unique<BinaryOperation>(operation, std::move(expression), std::move(right));
        }
        return expression;
    }

    ASTNodePtr Parser::Term()
    {
        ASTNodePtr expression = Factor();

        while (Match(shared::TokenType::TOKEN_PLUS, shared::TokenType::TOKEN_MINUS))
        {
            shared::TokenType operation = Previous().type;
            ASTNodePtr right = Factor();
            expression = std::make_unique<BinaryOperation>(operation, std::move(expression), std::move(right));
        }
        return expression;
    }

    ASTNodePtr Parser::Equality()
    {
        return ASTNodePtr();
    }

    ASTNodePtr Parser::Assignment()
    {
        ASTNodePtr expression = Term();

        if(Check(shared::TokenType::TOKEN_EQUAL))// && expression->GetType() == NodeType::NODE_ACCESSVARIABLE)
        {
            Expect(shared::TokenType::TOKEN_EQUAL, SyntaxExceptionCodes::EX_SYNTAX_MISSING_EQUAL, "Expected = for assignment.");
            ASTNodePtr value = Assignment();

            std::string name = static_cast<AccessVariableNode*>(expression.get())->GetName();
            return std::make_unique<AssignVariableNode>(name, std::move(value));
        }
        
        return expression;
    }

    ASTNodePtr Parser::Expression()
    {
        return Assignment();
    }

    ASTNodePtr Parser::ExpressionStatement()
    {
        ASTNodePtr expression = Expression();
        Expect(shared::TokenType::TOKEN_STATEMENT_END, SyntaxExceptionCodes::EX_SYNTAX_MISSING_SEMICOLON, "Semicolon (;) is required to end expressions.");
        return expression; 
    }

    ASTNodePtr Parser::Statement()
    {
        if(Match(shared::TokenType::TOKEN_LEFT_BRACE)) return BlockStatement();

        return ExpressionStatement();
    }

    ASTNodePtr Parser::Declaration()
    {
        if (Match(shared::TokenType::TOKEN_NATIVE_TYPE)) 
          return VarDeclaration();

        return Statement();
    }

    ASTNodePtr Parser::FunctionCall(std::string name, bool isNative)
    {
        Expect(shared::TokenType::TOKEN_LEFT_PARENTHESIS, SyntaxExceptionCodes::EX_SYNTAX_MISSING_LEFTPAREN, "Function call expected an opening paranthesis '('.");

        std::vector<ASTNodePtr> arguments;
        arguments.reserve(2); /** @todo An average of native function call arguments. */

        if(Peek().type != shared::TokenType::TOKEN_RIGHT_PARENTHESIS)
        {
          arguments.push_back(Expression());
          while(Peek().type == shared::TokenType::TOKEN_COMA)
          {
            Advance();
            arguments.push_back(Expression());
          }
        }
        
        Expect(shared::TokenType::TOKEN_RIGHT_PARENTHESIS, SyntaxExceptionCodes::EX_SYNTAX_MISSING_RIGHTPAREN, "You must enclose the function arguments with a closing paranthesis ')'.");
        return std::make_unique<FunctionCallNode>(name, std::move(arguments), isNative); 
    }

    ASTNodePtr Parser::BlockStatement()
    {
        std::vector<ASTNodePtr> statements;
        statements.reserve(3); /** @warning Predictional value. */

        while(!Check(shared::TokenType::TOKEN_RIGHT_BRACE) && !IsAtEnd())
        {
            statements.emplace_back(Declaration());
        }
            
        Expect(shared::TokenType::TOKEN_RIGHT_BRACE, SyntaxExceptionCodes::EX_SYNTAX_MISSING_RIGHTBRACE, "Expected a right brace '}'.");
        return std::make_unique<BlockNode>(std::move(statements));
    }

    ASTNodePtr Parser::VarDeclaration()
    {
        shared::Token identifier = Consume(shared::TokenType::TOKEN_IDENTIFIER, SyntaxExceptionCodes::EX_SYNTAX_MISSING_IDENTIFIER, "Variable declarations requires an identifier!");
            
        ASTNodePtr expression = nullptr;
        if(Match(shared::TokenType::TOKEN_EQUAL)){
            expression = Expression();
        }
            
        Expect(shared::TokenType::TOKEN_STATEMENT_END, SyntaxExceptionCodes::EX_SYNTAX_MISSING_SEMICOLON, "Varible declarations must end with a semicolon (;).");
        if(expression != nullptr) return std::make_unique<DeclareVariableNode>(identifier.lexeme, std::move(expression));
        return std::make_unique<ErrorNode>();
    }

}
