#include "snw_tst.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    void compare(Token lhs, Token rhs, bool pos = true)
    {
        assert(lhs.type == rhs.type);
        assert(lhs.content == rhs.content);
        if (pos) {
            assert(lhs.row == rhs.row);
            assert(lhs.col == rhs.col);
        }
    }
}

void Snowda::tstCompiler()
{
    TestModule module("Compiler");
    tstLexer();
    tstParser();
}

void Snowda::tstLexer()
{
    TestCase test("Lexer");
    {
        Lexer lexer("");
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 0));
    }
    {
        Lexer lexer("a bb ccc");
        compare(lexer.next(), Token(TokenType::Identifier, "a", 0, 0));
        compare(lexer.next(), Token(TokenType::Identifier, "bb", 0, 2));
        compare(lexer.next(), Token(TokenType::Identifier, "ccc", 0, 5));
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 8));
    }
    {
        Lexer lexer("a bb\nccc");
        compare(lexer.next(), Token(TokenType::Identifier, "a", 0, 0));
        compare(lexer.next(), Token(TokenType::Identifier, "bb", 0, 2));
        compare(lexer.next(), Token(TokenType::Newline, "\n", 0, 4));
        compare(lexer.next(), Token(TokenType::Identifier, "ccc", 1, 0));
        compare(lexer.next(), Token(TokenType::Finished, "", 1, 3));
    }
    {
        Lexer lexer("a bb\r\nccc");
        compare(lexer.next(), Token(TokenType::Identifier, "a", 0, 0));
        compare(lexer.next(), Token(TokenType::Identifier, "bb", 0, 2));
        compare(lexer.next(), Token(TokenType::Newline, "\r\n", 0, 4));
        compare(lexer.next(), Token(TokenType::Identifier, "ccc", 1, 0));
        compare(lexer.next(), Token(TokenType::Finished, "", 1, 3));
    }
    {
        Lexer lexer("\"1\n23\"\"abc\"");
        compare(lexer.next(), Token(TokenType::String, "1\n23", 0, 0));
        compare(lexer.next(), Token(TokenType::String, "abc", 1, 3));
        compare(lexer.next(), Token(TokenType::Finished, "", 1, 8));
    }
    {
        Lexer lexer("\"123\"\"abc\"");
        compare(lexer.next(), Token(TokenType::String, "123", 0, 0));
        compare(lexer.next(), Token(TokenType::String, "abc", 0, 5));
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 10));
    }
    {
        Lexer lexer("'a''b''c'");
        compare(lexer.next(), Token(TokenType::Character, "a", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Character, "b", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Character, "c", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 0), false);
    }
    {
        Lexer lexer("import class fn if else return");
        compare(lexer.next(), Token(TokenType::Import, "import", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Class, "class", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Fn, "fn", 0, 0), false);
        compare(lexer.next(), Token(TokenType::If, "if", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Else, "else", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Return, "return", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 0), false);
    }
    {
        Lexer lexer("== != >= <= << >> = < > ( ) [ ] { } & | ! ~ ^ * / % + - # ; : . ,");
        compare(lexer.next(), Token(TokenType::Equal, "==", 0, 0), false);
        compare(lexer.next(), Token(TokenType::NEqual, "!=", 0, 0), false);
        compare(lexer.next(), Token(TokenType::GEqual, ">=", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LEqual, "<=", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LShift, "<<", 0, 0), false);
        compare(lexer.next(), Token(TokenType::RShift, ">>", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Assign, "=", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LABrace, "<", 0, 0), false);
        compare(lexer.next(), Token(TokenType::RABrace, ">", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LParen, "(", 0, 0), false);
        compare(lexer.next(), Token(TokenType::RParen, ")", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LBrace, "[", 0, 0), false);
        compare(lexer.next(), Token(TokenType::RBrace, "]", 0, 0), false);
        compare(lexer.next(), Token(TokenType::LCBrace, "{", 0, 0), false);
        compare(lexer.next(), Token(TokenType::RCBrace, "}", 0, 0), false);
        compare(lexer.next(), Token(TokenType::And, "&", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Or, "|", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Bang, "!", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Tilde, "~", 0, 0), false);
        compare(lexer.next(), Token(TokenType::XOr, "^", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Mult, "*", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Div, "/", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Mod, "%", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Plus, "+", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Minus, "-", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Pound, "#", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Semi, ";", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Colon, ":", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Dot, ".", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Comma, ",", 0, 0), false);
        compare(lexer.next(), Token(TokenType::Finished, "", 0, 0), false);
    }
}

void Snowda::tstParser()
{
    tstParserRootStmt();
    tstParserFnStmt();
}

void Snowda::tstParserRootStmt()
{
    TestCase test("ParserRootStmt");
    {
        MemoryManager manager;
        Lexer lexer("(1 + 2) + 3");
        Parser parser(manager, lexer);
        RootResult result = parser.parseRootStatement();
        assert(result.hasValue());
        auto rootStmt = result.value();
    }
    {
        MemoryManager manager;
        Lexer lexer("(1 + 2) + 3\n");
        Parser parser(manager, lexer);
        RootResult result = parser.parseRootStatement();
        assert(result.hasValue());
        auto rootStmt = result.value();
    }
    {
        MemoryManager manager;
        Lexer lexer("(1 + 2) + 3\r\n");
        Parser parser(manager, lexer);
        RootResult result = parser.parseRootStatement();
        assert(result.hasValue());
        auto rootStmt = result.value();
    }
    {
        MemoryManager manager;
        Lexer lexer("(1 + 2) + 3;");
        Parser parser(manager, lexer);
        RootResult result = parser.parseRootStatement();
        assert(result.hasValue());
        auto rootStmt = result.value();
    }
    {
        MemoryManager manager;
        Lexer lexer("(1 + 2) + 3;(a + b)");
        Parser parser(manager, lexer);
        RootResult result = parser.parseRootStatement();
        if (result.hasError()) {
            ParserError error = result.error();
            std::cout << error << std::endl;
        }

        assert(result.hasValue());
        auto rootStmt = result.value();
        auto &stmts = rootStmt->stmts();

        assert(stmts.size() == 2);
        {
            auto stmt = stmts[0];
            assert(stmt->nodeType() == NodeType::ExprStmt);

            auto exprStmt = static_cast<const ExprStmt *>(stmt);
            {
                auto expr = exprStmt->expr();
                assert(expr->nodeType() == NodeType::BinaryExpr);

                auto binaryExpr = static_cast<const BinaryExpr *>(expr);
            }
        }
        {
            auto stmt = stmts[1];
            assert(stmt->nodeType() == NodeType::ExprStmt);

            auto exprStmt = static_cast<const ExprStmt *>(stmt);
            {
                auto expr = exprStmt->expr();
                assert(expr->nodeType() == NodeType::BinaryExpr);

                auto binaryExpr = static_cast<const BinaryExpr *>(expr);
                assert(binaryExpr->op() == BinaryExpr::Operator::Add);
                {
                    auto lhsExpr = binaryExpr->lhsExpr();
                    assert(lhsExpr->nodeType() == NodeType::IdentifierExpr);
                    
                    auto identifierExpr = static_cast<const IdentifierExpr *>(lhsExpr);
                    assert(identifierExpr->name() == "a");
                }
                {
                    auto rhsExpr = binaryExpr->rhsExpr();
                    assert(rhsExpr->nodeType() == NodeType::IdentifierExpr);

                    auto identifierExpr = static_cast<const IdentifierExpr *>(rhsExpr);
                    assert(identifierExpr->name() == "b");
                }
            }
        }
    }
}

void Snowda::tstParserFnStmt()
{
    TestCase test("ParserFnStmt");
    {
        MemoryManager manager;
        Lexer lexer("fn foo() {}");
        Parser parser(manager, lexer);

        StmtResult result = parser.parseStatement();
        if (result.hasError()) {
            std::cout << result.error() << std::endl;
            abort();
        }

        auto &stmt = *result.value();
        assert(stmt.nodeType() == NodeType::FunctionDeclStmt);

        auto &fnStmt = static_cast<const FunctionDeclStmt &>(stmt);
        assert(fnStmt.name() == "foo");
        assert(fnStmt.args().empty());
        assert(fnStmt.block()->stmts().empty());
    }
    {
        MemoryManager manager;
        Lexer lexer("fn foo(a: Int32, b: Int64) { return a + b }");
        Parser parser(manager, lexer);

        StmtResult result = parser.parseStatement();
        if (result.hasError()) {
            std::cout << result.error() << std::endl;
            abort();
        }

        // auto &stmt = *result.value();
        // assert(stmt.nodeType() == NodeType::FunctionDeclStmt);

        // auto &fnStmt = static_cast<const FunctionDeclStmt &>(stmt);
        // assert(fnStmt.name() == "foo");
        // assert(fnStmt.args().empty());
        // assert(fnStmt.block()->stmts().empty());
    }
}
