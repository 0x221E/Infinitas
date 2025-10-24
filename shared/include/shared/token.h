#pragma once

#include <string>
#include <cstdint>

namespace shared
{
    enum class TokenType : uint32_t
    {
        TOKEN_IDENTIFIER = 0,
        TOKEN_INTEGER_LITERAL = 10,
        TOKEN_FLOAT_LITERAL = 11,
        TOKEN_STRING_LITERAL = 12,
        TOKEN_PLUS = 100,
        TOKEN_MINUS = 101,
        TOKEN_ASTERISK = 102,
        TOKEN_SLASH = 103,
        TOKEN_EQUAL = 104,
        TOKEN_EQUAL_EQUAL = 110,
        TOKEN_BANG_EQUAL = 111,
        TOKEN_NATIVE_FUNCTION = 300, // SPEC 4.3.*
        TOKEN_NATIVE_TYPE = 301, // SPEC 4.2.*
        TOKEN_LEFT_PARENTHESIS = 400,
        TOKEN_RIGHT_PARENTHESIS = 401,
        TOKEN_COMA = 402,
        TOKEN_LEFT_BRACE = 403,
        TOKEN_RIGHT_BRACE = 404,
        TOKEN_STATEMENT_END = 999,
        TOKEN_END = 1000
    };

    struct Token
    {
        TokenType type;
        std::string lexeme;

        Token(TokenType _type, const std::string& _value) : type(_type), lexeme(_value) { }
        Token(TokenType _type, const char* _value) : type(_type), lexeme(_value) { }
        Token() = default;
    };

}
