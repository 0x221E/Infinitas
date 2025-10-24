/**
 * @file tokenizer.h
 * @author 0x221E
 * @date Created, 25 September 2025. Not Modified.
 * 
 * You can read more about my design choices at <URL>.
 */

#pragma once

#include <shared/exception.h>
#include <shared/error_context.h>
#include "tokenizer/exception_lexer.h"
#include <vector>
#include <string>
#include <expected>
#include <type_traits>
#include <unordered_map>
#include <iostream>

#include <shared/token.h>

namespace lexer{
    
    /**
     * Storage of all keywords. View translation unit for the exhaustive list.
     * The keywords are governed by the community at <URL>.
     */
    
    extern const std::unordered_map<std::string, shared::TokenType> s_Keywords;
    
    struct LexerErrorReturn
    {
      bool m_Error;
    };

    /**
     * @details Processes the source code for tokenization.
     * 
     * First step of the language workflow.
     */
    class Tokenizer{
    public:
        /**
         * @name Tokenizer
         * @brief Constructs tokenizer object.
         */
        Tokenizer();

        /**
         * @param std::string sourceCode to input to the tokenizer.
         * @brief Supplies the sourceCode to be tokenized.
         * 
         * @warning Resets the current state of the tokenizer.
         */
        void SupplyInput(const std::string& sourceCode);
        
        /**
         * @brief Gets the next token in line. 
         * @returns std::expected<shared::Token, LexerErrorReturn>Type, shared::TokenType::END if the source code is exhausted. 
         * 
         * Manages the tokenization order as well.
         */
        std::expected<shared::Token, LexerErrorReturn> GetNextToken();
        
        std::expected<std::vector<shared::Token>, bool> FetchAllTokens(const std::string& sourceCode);
        
    private:
        /** Class Members */

        /**
         * The raw source code string to be tokenized.
         */
        std::string m_SourceCode;

        /**
         * The current position of the tokenizer in the source code.
         */
        std::size_t m_CurrentPosition;
    
        shared::ErrorContext<LexerException> m_ErrorContext;

    private:
        /** Built-in Functionality */

        /**
         * @brief Gets the current character in tokenizer, determined by the current position index.
         * @returns char Character to be returned.
         */
        char Peek() const;

        /**
         * @brief Advances the current position of the tokenizer by one.
         * @todo Consider adding inline keyword.
         */
        void Advance();
    
    private:
        /** Fetch Operations */
        
        void SkipWhitespace();

        shared::Token StatementEnd();

        /**
         * @brief The core scanning logic. 
         */
        std::expected<shared::Token, LexerErrorReturn> ScanToken();
      
        /**
         * @brief Enforces spec 4.3 and 4.2 | Searches for primitive types and functions.
         * @return Word to be returned as a native, or error.
         */
        std::expected<shared::Token, LexerErrorReturn> FindNatives();

        /**
         * @brief Enforces spec 4.3 | Determines whether the characters next in line belong to a call to a native function.
         * @return Token to be returned with the type native function.
         */
        shared::Token FetchNativeFunction();
        
        /**
         * @brief Enforces spec 4.2 | Determines whether the characters next in line belong to a call to a native type.
         * @return Token to be returned with the type native type.
         */
        shared::Token FetchNativeType();
          
        /**
         * @brief Fetches the entire word that consists of alpha characters.
         * @return  Word to be returned as a keyword token, or error.
         * 
         * @warning Should only be used when the current character is an alpha character.
         * @warning Advances current position to the end of the word.
         */
        std::expected<shared::Token, LexerErrorReturn> FindKeyword();

        /**
         * @brief Fetches the entire number.
         * @return std::expected<shared::Token, LexerErrorReturn> Number to be returned as an integer literal.
         * 
         * @warning Should only be used when the current character is a digit.
         * @warning Advances current position to the end of the number.
         */
        std::expected<shared::Token, LexerErrorReturn> FetchNumber();

        /**
         * @brief Fetches the entire string literal.
         * @return std::expected<shared::Token, LexerErrorReturn> String to be returned as string literal.
         * 
         * @warning Should only be used when the current character is a quotation mark.
         * @warning Advances current position to the end of the number.
         */
        std::expected<shared::Token, LexerErrorReturn> FetchString();

    private:
        /** Checks */

        /**
         * @brief Checks for special-symbol operators.
         * @return std::expected<shared::Token, LexerErrorReturn> Token with the operator to be returned.
         * 
         * @warning Advances current position to after operator.
         */
        std::expected<shared::Token, LexerErrorReturn> CheckOperators();    
    };

}
