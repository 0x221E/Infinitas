#include <iostream>
#include <gtest/gtest.h>
#include <parser/parser.h>
#include <memory>

struct ParserTestCase
{
    std::string m_Name;
    std::vector<shared::Token> m_Tokens;
    std::vector<std::shared_ptr<parser::ASTNode>> m_ExpectTree;

    ParserTestCase(std::string name, std::vector<shared::Token> tokens,
                   std::vector<std::shared_ptr<parser::ASTNode>> &&expectTree)
        : m_Name(name), 
        m_Tokens(tokens), 
        m_ExpectTree(std::move(expectTree))
    {
    }
};

class ParserTest : public testing::TestWithParam<ParserTestCase>
{
protected:
    std::vector<std::unique_ptr<parser::ASTNode>> ParseTokens(const std::vector<shared::Token> &tokens)
    {
        parser::Parser treeParser;
        return treeParser.Parse(tokens);
    }
};

void RecursiveTest(std::unique_ptr<parser::ASTNode> &&node, const std::shared_ptr<parser::ASTNode>& expect)
{
    switch (node->GetType())
    {
        EXPECT_EQ(node->GetType(), expect->GetType()) << "Type does not match! Actual: INT";

    case parser::NodeType::NODE_INTLITERAL:
    {
        parser::IntegerLiteralNode *actual = static_cast<parser::IntegerLiteralNode *>(node.get());
        parser::IntegerLiteralNode *expected = static_cast<parser::IntegerLiteralNode *>(expect.get());
        EXPECT_EQ(actual->GetValue(), expected->GetValue()) << "Integer values do not match.";
        break;
    }

    case parser::NodeType::NODE_FLOATLITERAL:
    {
        parser::FloatLiteralNode *actual = static_cast<parser::FloatLiteralNode *>(node.get());
        parser::FloatLiteralNode *expected = static_cast<parser::FloatLiteralNode *>(expect.get());
        EXPECT_EQ(actual->GetValue(), expected->GetValue()) << "Float values do not match the 50 digit precision level.";
        break;
    }

    case parser::NodeType::NODE_GROUPING:
    {
        parser::GroupingExpression *actual = static_cast<parser::GroupingExpression *>(node.get());
        parser::GroupingExpression *expected = static_cast<parser::GroupingExpression *>(expect.get());
        RecursiveTest(std::move(actual->GetExpression()), std::shared_ptr<parser::ASTNode>(expect, expected->m_Expression.get()));
        break;
    }

    case parser::NodeType::NODE_BINARY:
    {
        parser::BinaryOperation *actual = static_cast<parser::BinaryOperation *>(node.get());
        parser::BinaryOperation *expected = static_cast<parser::BinaryOperation *>(expect.get());
        EXPECT_EQ(actual->m_Operator, expected->m_Operator);
        RecursiveTest(std::move(actual->m_Left), std::shared_ptr<parser::ASTNode>(expect, expected->m_Left.get()));
        RecursiveTest(std::move(actual->m_Right), std::shared_ptr<parser::ASTNode>(expect, expected->m_Right.get()));
        break;
    }

    case parser::NodeType::NODE_PRINT:
    {
        parser::PrintNode *actual = static_cast<parser::PrintNode *>(node.get());
        parser::PrintNode *expected = static_cast<parser::PrintNode *>(expect.get());
        RecursiveTest(std::move(actual->m_Expression), std::shared_ptr<parser::ASTNode>(expect, expected->m_Expression.get()));
        break;
    }
    
    case parser::NodeType::NODE_EXIT:
    {
        parser::ExitNode *actual = static_cast<parser::ExitNode *>(node.get());
        parser::ExitNode *expected = static_cast<parser::ExitNode *>(expect.get());
        break;
    }

    default:
    {
        ASSERT_ANY_THROW("Nope!");
    }
    }
}

TEST_P(ParserTest, ParserTestSingle)
{
    auto &params = GetParam();
    auto actualResult = ParseTokens(params.m_Tokens);

    EXPECT_EQ(actualResult.size(), params.m_ExpectTree.size());

    for (int i = 0; i < actualResult.size(); i++)
    {
        RecursiveTest(std::move(actualResult[i]), params.m_ExpectTree[i]);
    }
}

INSTANTIATE_TEST_SUITE_P(
    ParserTests_Positive,
    ParserTest,
    ::testing::Values(
        ParserTestCase{
            "Integer_SimpleTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::IntegerLiteralNode>(5)
            }
        },
        ParserTestCase{
            "Integer_LargeTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "99999999"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::IntegerLiteralNode>(99999999)
            }
        },
        ParserTestCase{
            "Float_SimpleTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.57"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::FloatLiteralNode>(mp::mpf_float("5.57"))
            }
        },
        ParserTestCase{
            "Float_LargeTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "99999999.53"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::FloatLiteralNode>(mp::mpf_float("99999999.53"))
            }
        },
        ParserTestCase{
            "Binary_SimpleIntegerAdditionTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                    std::make_unique<parser::IntegerLiteralNode>(5), 
                    std::make_unique<parser::IntegerLiteralNode>(5))}
            }
        },
        ParserTestCase{
            "Binary_SimpleIntegerSubstractionTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_MINUS,
                    std::make_unique<parser::IntegerLiteralNode>(5), 
                    std::make_unique<parser::IntegerLiteralNode>(5))}
            }
        },
        ParserTestCase{
            "Binary_SimpleIntegerMultiplicationTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                    std::make_unique<parser::IntegerLiteralNode>(5), 
                    std::make_unique<parser::IntegerLiteralNode>(5))}
            }
        },
        ParserTestCase{
            "Binary_SimpleIntegerDivisionTest",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_SLASH,
                    std::make_unique<parser::IntegerLiteralNode>(5), 
                    std::make_unique<parser::IntegerLiteralNode>(5))}
            }
        },
        ParserTestCase{
            "Binary_SimpleFloatAdditionTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")))}
            }
        },
        ParserTestCase{
            "Binary_SimpleFloatSubstractionTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_MINUS,
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")))}
            }
        },
        ParserTestCase{
            "Binary_SimpleFloatMultiplicationTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")))}
            }
        },
        ParserTestCase{
            "Binary_SimpleFloatDivisionTest",
            {
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_SLASH,
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                    std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")))}
            }
        },
        ParserTestCase{
            "Grouping_SimpleIntegerTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::GroupingExpression>(std::make_unique<parser::IntegerLiteralNode>(5))}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleIntegerAdditionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_PLUS,
                        std::make_unique<parser::IntegerLiteralNode>(5), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    )
                )
            }
            }
            
        },
        ParserTestCase{
            "GroupingBinary_SimpleIntegerSubstractionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_MINUS,
                        std::make_unique<parser::IntegerLiteralNode>(5), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    )
                )}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleIntegerMultiplicationTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_STAR,
                        std::make_unique<parser::IntegerLiteralNode>(5), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    )
                )}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleIntegerDivisionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_SLASH,
                        std::make_unique<parser::IntegerLiteralNode>(5), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    )
                )}
            }
        },
        ParserTestCase{
            "Grouping_SimpleFloatTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {std::make_shared<parser::GroupingExpression>(std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")))}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleFloatAdditionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_PLUS,
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35"))
                    )
                )
            }
            }
            
        },
        ParserTestCase{
            "GroupingBinary_SimpleFloatSubstractionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_MINUS,
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35"))
                    )
                )}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleFloatMultiplicationTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_STAR,
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35"))
                    )
                )}
            }
        },
        ParserTestCase{
            "GroupingBinary_SimpleFloatDivisionTest",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.35"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                {
                    std::make_shared<parser::GroupingExpression>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_SLASH,
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35")), 
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("5.35"))
                    )
                )}
            }
        },
        ParserTestCase{
            "Precedence_HighAssociativity",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "2"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                    std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_SLASH,
                        std::make_unique<parser::IntegerLiteralNode>(10), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    ),
                    std::make_unique<parser::IntegerLiteralNode>(2)
                )
            }
        },
        ParserTestCase{
            "Precedence_LowAssociativity_MixedOps",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "2"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_MINUS,
                    std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                        std::make_unique<parser::IntegerLiteralNode>(10), 
                        std::make_unique<parser::IntegerLiteralNode>(5)
                    ),
                    std::make_unique<parser::IntegerLiteralNode>(2)
                )
            }
        },
        ParserTestCase{
            "Precedence_ComplexNesting",
            {
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "1"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "2"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "3"},
                {shared::TokenType::TOKEN_MINUS, "-"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "4"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                    std::make_unique<parser::GroupingExpression>(
                        std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                            std::make_unique<parser::IntegerLiteralNode>(1), 
                            std::make_unique<parser::IntegerLiteralNode>(2)
                        )
                    ),
                    std::make_unique<parser::GroupingExpression>(
                        std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_MINUS,
                            std::make_unique<parser::IntegerLiteralNode>(3), 
                            std::make_unique<parser::IntegerLiteralNode>(4)
                        )
                    )
                )
            }
        },
        ParserTestCase{
            "Precedence_Triple_Order",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "10"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "2"},
                {shared::TokenType::TOKEN_SLASH, "/"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "4"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                    std::make_unique<parser::IntegerLiteralNode>(10), 
                    std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_SLASH,
                        std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                            std::make_unique<parser::IntegerLiteralNode>(5), 
                            std::make_unique<parser::IntegerLiteralNode>(2)
                        ),
                        std::make_unique<parser::IntegerLiteralNode>(4)
                    )
                )
            }
        },
        ParserTestCase{
            "Precedence_MixedFloatAndInt",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "3"},
                {shared::TokenType::TOKEN_STAR, "*"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "2.5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::BinaryOperation>(shared::TokenType::TOKEN_PLUS,
                    std::make_unique<parser::IntegerLiteralNode>(5), 
                    std::make_unique<parser::BinaryOperation>(shared::TokenType::TOKEN_STAR,
                        std::make_unique<parser::IntegerLiteralNode>(3),
                        std::make_unique<parser::FloatLiteralNode>(mp::mpf_float("2.5"))
                    )
                )
            }
        },
        ParserTestCase{
            "Statement_PrintLiteral",
            {
                {shared::TokenType::TOKEN_KEYWORD_PRINT, "print"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::PrintNode>(
                    std::make_unique<parser::IntegerLiteralNode>(5)
                )
            }
        },
        ParserTestCase{
            "Statement_PrintExpression",
            {
                {shared::TokenType::TOKEN_KEYWORD_PRINT, "print"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "1"},
                {shared::TokenType::TOKEN_PLUS, "+"},
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "2"},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::PrintNode>(
                    std::make_unique<parser::BinaryOperation>(
                        shared::TokenType::TOKEN_PLUS,
                        std::make_unique<parser::IntegerLiteralNode>(1), 
                        std::make_unique<parser::IntegerLiteralNode>(2)
                    )
                )
            }
        },
        ParserTestCase{
            "Statement_Exit",
            {
                {shared::TokenType::TOKEN_KEYWORD_EXIT, "exit"},
                {shared::TokenType::TOKEN_LEFT_PARENTHESIS, "("},
                {shared::TokenType::TOKEN_RIGHT_PARENTHESIS, ")"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::ExitNode>()
            }
        },
        ParserTestCase{
            "MultipleStatements_Simple",
            {
                {shared::TokenType::TOKEN_INTEGER_LITERAL, "5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_FLOAT_LITERAL, "5.5"},
                {shared::TokenType::TOKEN_STATEMENT_END, ";"},
                {shared::TokenType::TOKEN_END, ""},
            },
            {
                std::make_shared<parser::IntegerLiteralNode>(5),
                std::make_shared<parser::FloatLiteralNode>(mp::mpf_float("5.5"))
            }
        }
    ),
    [](const testing::TestParamInfo<ParserTestCase>& info){
        return "Case_" + info.param.m_Name;
    }
);