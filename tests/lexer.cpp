#include <tokenizer/tokenizer.h>
#include <gtest/gtest.h>
#include <iostream>

struct LexTestCase
{
    std::string name;
    std::string sourceCode;
    std::vector<shared::Token> expected;
};

void PrintTo(const LexTestCase& testCase, std::ostream* os) {
    *os << "LexTestCase<" << testCase.name << ">, Source<" << testCase.sourceCode << ">";
}

class LexerTest : public testing::TestWithParam<LexTestCase>
{
protected:
    std::vector<shared::Token> TokenizeInput(const std::string &input) const
    {
        lexer::Tokenizer tokenizer;

        tokenizer.SupplyInput(input);

        std::vector<shared::Token> tokens;
        tokens.reserve(input.length());

        shared::Token currentToken;
        do
        {
            currentToken = tokenizer.GetNextToken();
            tokens.emplace_back(currentToken);
        } while (currentToken.type != shared::TokenType::TOKEN_END);

        return tokens;
    }
};

TEST_P(LexerTest, TokenizationFlowMatchesExpected)
{
    auto &params = GetParam();
    auto actualResult = TokenizeInput(params.sourceCode);
    ASSERT_EQ(actualResult.size(), params.expected.size()) << "Size check failed for input: " << params.sourceCode;

    for (std::size_t i = 0; i < actualResult.size(); i++)
    {
        EXPECT_EQ(actualResult[i].type, params.expected[i].type) << "Index at " << i;
        EXPECT_EQ(actualResult[i].lexeme, params.expected[i].lexeme) << "Index at " << i;
    }
}

INSTANTIATE_TEST_SUITE_P(
    IntegerArithmeticTests,
    LexerTest,
    ::testing::Values(
        LexTestCase{
            "SimpleAddition",
            "15 + 15;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "SimpleSubstraction",
            "15 - 15;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "ComplexADDSUB",
            "99999 + 99999 - 97777;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "99999"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "99999"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "97777"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "SimpleMultiplication",
            "15 * 15;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_ASTERISK, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "SimpleDivision",
            "15 / 15;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "15"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "ComplexMulDiv",
            "20 * 20 / 10;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "20"},
                {shared::TokenType::TOKEN_ASTERISK, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "20"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "ComplexAddSubMulDiv",
            "10 + 10 * 30 - 10 / 10;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_ASTERISK, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "30"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        }
    ),
    [](const testing::TestParamInfo<LexTestCase>& info){
        return "case_" + info.param.name;
    }
);

INSTANTIATE_TEST_SUITE_P(
    KeywordTests,
    LexerTest,
    ::testing::Values(
        LexTestCase{
            "PrintComplexExpr",
            "print(5+5-(5-5));",
            {
                {shared::TokenType::TOKEN_KEYWORD_PRINT, "print"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "PrintSpacing",
            "print (5);",
            {
                {shared::TokenType::TOKEN_KEYWORD_PRINT, "print"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "PrintWithNewLine",
            "print \n (5);",
            {
                {shared::TokenType::TOKEN_KEYWORD_PRINT, "print"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        }
    ),
    [](const testing::TestParamInfo<LexTestCase>& info){
        return "Case_" + info.param.name;
    }
);

INSTANTIATE_TEST_SUITE_P(
    UnaryTests,
    LexerTest,
    ::testing::Values(
        LexTestCase{
            "ParanthesesTest1",
            "(5+5-(5-5));",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        }
    ),
    [](const testing::TestParamInfo<LexTestCase>& info){
        return "Case_" + info.param.name;
    }
);


INSTANTIATE_TEST_SUITE_P(
    SpacingAndErrorTests,
    LexerTest,
    ::testing::Values(
        LexTestCase{
            "EmptySpace",
            "",
            {
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "TrailingSpaces",
            "     1;      ",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "1"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "TrailingSpacesWithNewline",
            "   10      \n     ;",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "MultipleStatements",
            "     1; 3; 5; 10;     ",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "1"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "3"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        },
        LexTestCase{
            "MultipleStatementEnds",
            "     ;;;;     ",
            {
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""}
            }
        }
    ),
    [](const testing::TestParamInfo<LexTestCase>& info){
        return "Case_" + info.param.name;
    }
);