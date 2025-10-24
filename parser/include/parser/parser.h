/**
 * @file parser.h
 * @author 0x221E
 * @brief Header file containing the recursive descent parser for the language.
 * @brief Processes tokens and converts them to AST Trees.
 */

#pragma once

#include <type_traits>
#include <vector>
#include "nodes/nodes.h"
#include "parser/exception_syntax.h"
#include <shared/error_context.h>
#include <expected>

namespace parser
{
    /* 
     * @name Parser
     * @brief Processes tokens, converts to Abstract Syntax Trees (AST)
     * 
     * Uses Recursive Descent Parser (RDP) to generate the AST.
     */
    class Parser
    {
    public:
    /** Constructor/Destructor */

        /**
         * @brief Constructs a parser instance.
         */
        Parser();

    public:
    /** Public Functions */

        /**
         * @brief Parses the supplied tokens.
         * @param std::vector<shared::Token> Vector of tokens to be parsed.
         * @returns std::vector<ASTNodePtr>, Abstract Syntax Tree
         * 
         * @warning Resets the current state of the parser.
         */
        std::expected<std::vector<ASTNodePtr>, bool> Parse(std::vector<shared::Token> tokens);

    private:
    /** Private Variables */
        
        /**
         * @brief Vector of tokens that are supplied through Parser::Parse().
         * @warning Resets each use of Parser::Parse().
         */
        std::vector<shared::Token> m_Tokens;

        /**
         * @brief Current position of the active parse operation.
         * @warning Resets each use of Parser::Parse().
         */
        std::size_t m_CurrentPosition;
        
        /**
         * @warning Error context resets on each error-result during Read-Eval-Print-Loop.
         */
        shared::ErrorContext<SyntaxException> m_ErrorContext;

    private:
        /** Helper functions */

        /**
         * @brief Resets the current state.
         * 
         * @warning Should not be called anywhere except Parser::Parse().
         */
        void Cleanup();

        /**
         * @brief Fetches the actively parsed token.
         * @returns shared::Token token at the current position of current parser state.
         */
        shared::Token Peek();

        /**
         * @brief Fetches the previous token.
         * @returns shared::Token token at the previous position of current parser state.
         */
        shared::Token Previous();

        /**
         * @brief Checks whether the current token is shared::TokenType::END
         * @returns bool
         */
        bool IsAtEnd();

        /**
         * @brief Increments the current position of the current parser state.
         * @returns shared::Token, the current token.
         */
        shared::Token Advance();

        /**
         * @brief Checks whether the supplied token is the current token.
         * @returns bool
         */
        bool Check(shared::TokenType type);

        /**
         * @brief Expects the supplied token type to be the current token, if not, throws a syntax error.
         * @param Token to be passed.
         * @param errorMessage Error message to be thrown on failure.
         */
        void Expect(shared::TokenType expectedToken, SyntaxExceptionCodes errorCode, std::string errorMessage);

        /**
         * @brief Expects the supplied token type to be the next token and consumes it, if not, throws a syntax error.
         * @param Token to be expected and consumed.
         * @param Error message to be thrown on failure.
         */
        shared::Token Consume(shared::TokenType expectedToken, SyntaxExceptionCodes errorCode, std::string errorMessage);

        /**
         * @brief Checks whether any of the supplied types matches the token at current position.
         * @tparam T shared::TokenType enum value.
         * @param T... All shared::TokenType to be checked.
         * @returns bool, whether the current token matches or not.
         */
        template <typename... T>
        bool Match(T... types)
        {
            static_assert((std::is_same_v<T, shared::TokenType> && ...), "Must be a shared::TokenType enum!");

            for (shared::TokenType type : {types...})
            {
                if (Check(type))
                {
                    Advance();
                    return true;
                }
            }
            return false;
        }

    private:
    /** Grammar Functions, ordered by simplest to most complex. */
        
        /**
         * @brief Holds the most primitive types possible. This is the lowest ring in expressions.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Primary();
        
        /**
         * @brief Turns bang (!) and minus (-) tokens into AST Nodes.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Unary();


        /** @todo Add (--) operator between Unary() and Factor() */

        /**
         * @brief Handles multiplication and division arithmetic operations. Turns star (*) and slash (/) tokens into AST Nodes.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Factor();

        /**
         * @brief  Handles addition and substraction arithmetic operations. Turns plus (+) and minus (-) tokens into binary operation AST Nodes.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Term();

        /**
         * @todo NOT IMPLEMENTED YET.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Equality();

        /**
         * @brief Handles the assignment operation of variables.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Assignment();
        
        /**
         * @brief Wrapper function that calls Assignment.
         * @returns ASTNodePtr AST Node
         */
        ASTNodePtr Expression();

        /**
         * @brief Handles expression statements, checks for semicolon (;).
         * @returns ASTNodePtr AST Node.
         */
        ASTNodePtr ExpressionStatement();

        /**
         * @brief Handles all the native functions.
         * Currently supported ones:
         *      print, exit
         * @returns ASTNodePtr AST Node.
         */
        ASTNodePtr Statement();

        /**
         * @brief Handles all declarations.
         * Currently supported ones:
         *      var
         * @returns ASTNodePtr AST Node.
         */
        ASTNodePtr Declaration();
        
    private:
    /** Function Statements */
        
        /**
         * @brief Converts the incoming tokens to function call AST Node.
         * 
         * @warning Instantiates a new error handler scope.
         */
        ASTNodePtr FunctionCall(std::string name, bool isNative);
 
        ASTNodePtr BlockStatement();

    /** Declaration Statements */

        /**
         * @brief Converts the incoming tokens to var declaration AST Node.
         * 
         * @warning Instantiates a new error handler scope.
         */
        ASTNodePtr VarDeclaration();

    };

}
