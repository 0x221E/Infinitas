/**
 * @file tokenizer.cpp
 * @author 0x221E
 * @date Created, 25 September 2025. Not Modified.
 */

#include "tokenizer/tokenizer.h"
#include "tokenizer/exception_lexer.h"

#include <string>
#include <cctype>

#include <shared/logger.h>

namespace lexer
{

    const std::unordered_map<std::string, shared::TokenType> s_Keywords = 
    {
    };

    Tokenizer::Tokenizer() : m_SourceCode{""}, m_CurrentPosition{0}
    {

    }

    void Tokenizer::SupplyInput(const std::string& sourceCode)
    {
        m_CurrentPosition = 0;
        m_SourceCode = sourceCode;
    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::GetNextToken()
    {
        SkipWhitespace();

        if (m_CurrentPosition >= m_SourceCode.length())
        {
            return shared::Token{shared::TokenType::TOKEN_END, ""};
        }

        return ScanToken();
    }

    /**
     * @todo Optimize and organize this function!
     */
    std::expected<std::vector<shared::Token>, bool> Tokenizer::FetchAllTokens(const std::string &input)
    {
        std::vector<shared::Token> tokens;
        tokens.reserve(input.length()); /** @todo Implement a better reserve logic. */
        SupplyInput(input);

        bool errorThrown = false;
      
        while(true)
        {
            auto result = GetNextToken();
          
            if(result.has_value())
            {
                auto token = result.value();

                LOG_DEBUG("token: {}, {}", std::to_string(static_cast<uint32_t>(token.type)), token.lexeme);

                if(token.type == shared::TokenType::TOKEN_END)
                {  
                    tokens.emplace_back(std::move(token));
                    break;
                }

                tokens.emplace_back(std::move(token));
            }

            if(!result.has_value() && result.error().m_Error == true)
            {
                errorThrown = true;
            } 
      }

      if(errorThrown) return std::unexpected(true);
      return tokens;
  }

    char Tokenizer::Peek() const
    {
        return m_SourceCode[m_CurrentPosition];
    }

    void Tokenizer::Advance()
    {
        m_CurrentPosition++;
    }

    void Tokenizer::SkipWhitespace()
    {
        while (m_CurrentPosition < m_SourceCode.length() 
            && std::isspace(m_SourceCode[m_CurrentPosition]))
        {
            Advance();
        }
    }

    shared::Token Tokenizer::StatementEnd()
    {
      Advance();
      return shared::Token{shared::TokenType::TOKEN_STATEMENT_END, ";"};
    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::ScanToken()
    {
      const char currentCharacter = Peek();

        switch(currentCharacter)
        {
          case '_':  
            return FindNatives();
          case '"': 
            return FetchString();
          case ';':
            return StatementEnd(); 
          default:
          {
            if(std::isalpha(currentCharacter))
            {
              return FindKeyword();
            }

            if (std::isdigit(currentCharacter))
            {
                return FetchNumber();
            }
              
            return CheckOperators();
          }
        }

    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::FindNatives()
    {
        int underscoreCount = 0;

        while (m_CurrentPosition + underscoreCount < m_SourceCode.size()
              && m_SourceCode[m_CurrentPosition + underscoreCount] == '_')
        {
            underscoreCount++;
        }
        
        m_CurrentPosition += underscoreCount;
    
        if(underscoreCount == 2)
        {
          return FetchNativeFunction(); 
        }
        else if (underscoreCount == 3)
        {
          return FetchNativeType();
        }
        
        m_ErrorContext.LogError(LexerExceptionCodes::EX_LEX_CHAR_MISPLACED, "'_' is misplaced.");
        return std::unexpected<LexerErrorReturn>(true);
    }

    shared::Token Tokenizer::FetchNativeFunction()
    {
        int i = 0;

        while (m_CurrentPosition + i < m_SourceCode.size() 
                && (std::isalnum(m_SourceCode[m_CurrentPosition + i]) || m_SourceCode[m_CurrentPosition + i] == '_'))
        {
          i++;
        }
        
        std::string substring = m_SourceCode.substr(m_CurrentPosition, i);
    
        m_CurrentPosition = m_CurrentPosition + i;
        return shared::Token{shared::TokenType::TOKEN_NATIVE_FUNCTION, substring};
    }

    shared::Token Tokenizer::FetchNativeType()
    {
        int i = 0;

        while (m_CurrentPosition + i < m_SourceCode.size() 
                && (std::isalnum(m_SourceCode[m_CurrentPosition + i]) || m_SourceCode[m_CurrentPosition + i] == '_'))
        {
          i++;
        }
        
        std::string substring = m_SourceCode.substr(m_CurrentPosition, i);
    
        m_CurrentPosition = m_CurrentPosition + i;
        return shared::Token{shared::TokenType::TOKEN_NATIVE_TYPE, substring};
    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::FindKeyword()
    {
        int i = 0;
        while (m_CurrentPosition + i < m_SourceCode.size() 
                && std::isalpha(m_SourceCode[m_CurrentPosition + i]))
        {
          i++;
        }

        std::string substring = m_SourceCode.substr(m_CurrentPosition, i);
        auto keyword = s_Keywords.find(substring);

        if (keyword == s_Keywords.end())
        {
          m_CurrentPosition = m_CurrentPosition + i;
          return shared::Token{shared::TokenType::TOKEN_IDENTIFIER, substring};
        }

        m_CurrentPosition = m_CurrentPosition + i;
        return shared::Token{keyword->second, keyword->first};
    }

    /**
    * @todo Fix semantics, comply with Infinitas Standard Convention. 
    */
    std::expected<shared::Token, LexerErrorReturn> Tokenizer::FetchNumber()
    {
        int i = 0;
        while (m_CurrentPosition + i < m_SourceCode.size() 
                && std::isdigit(m_SourceCode[m_CurrentPosition + i]))
        {
            i++;
        }

        if (m_SourceCode[m_CurrentPosition + i] != '.')
        {
            auto number = m_SourceCode.substr(m_CurrentPosition, i);
            m_CurrentPosition = m_CurrentPosition + i;
            return shared::Token{shared::TokenType::TOKEN_INTEGER_LITERAL, number};
        }

        i++;

        while (m_CurrentPosition + i < m_SourceCode.size() 
                && std::isdigit(m_SourceCode[m_CurrentPosition + i]))
        {
            i++;
        }

        auto number = m_SourceCode.substr(m_CurrentPosition, i);
        m_CurrentPosition = m_CurrentPosition + i;
        return shared::Token{shared::TokenType::TOKEN_FLOAT_LITERAL, number};
    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::FetchString()
    {
        int i = 0;
        Advance();

        while(m_CurrentPosition + i < m_SourceCode.size() 
            && m_SourceCode[m_CurrentPosition + i] != '"')
        {
            i++;
        }

        std::string literal{m_SourceCode.substr(m_CurrentPosition, i)};

        m_CurrentPosition = m_CurrentPosition + i;
        Advance();

        return shared::Token{shared::TokenType::TOKEN_STRING_LITERAL, literal};
    }

    std::expected<shared::Token, LexerErrorReturn> Tokenizer::CheckOperators()
    {
        const char currentCharacter = Peek();

        switch (currentCharacter)
        {
        case '+':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_PLUS, std::string(1, currentCharacter)};
        case '-':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_MINUS, std::string(1, currentCharacter)};
        case '*':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_ASTERISK, std::string(1, currentCharacter)};
        case '/':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_SLASH, std::string(1, currentCharacter)};
         case ',':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_COMA, std::string(1, currentCharacter)};
        case '(':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_LEFT_PARENTHESIS, std::string(1, currentCharacter)};
        case ')':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_RIGHT_PARENTHESIS, std::string(1, currentCharacter)};
        case '=':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_EQUAL, std::string(1, currentCharacter)};
        case '{':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_LEFT_BRACE, std::string(1, currentCharacter)};
        case '}':
            Advance();
            return shared::Token{shared::TokenType::TOKEN_RIGHT_BRACE, std::string(1, currentCharacter)};
        default:
            Advance();
            m_ErrorContext.LogError(LexerExceptionCodes::EX_LEX_CHAR_UNKNOWN, "Unknown character.");
            return std::unexpected<LexerErrorReturn>(true);
        }
    }

}
