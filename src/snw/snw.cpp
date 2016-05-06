#include "snw.h"

using namespace Snowda;
using namespace Snowda::Ast;

void testLexer()
{
    {
        Lexer lexer("(a bb ccc)\n"
                    "(\"stuff\")\n"
                    "var a.type = bb;");

        for (;;) {
            Token token = lexer.next();
            std::cout << token << std::endl;
            if (token.type == TokenType::Finished || token.type == TokenType::Error) {
                break;
            }
        }
    }
}

void testTokenStream()
{
    Lexer lexer("(a b c);");
    TokenStream stream(lexer);

    size_t index = 0;
    auto checkNext = [&](TokenType expectedType) {
        Token token = stream[index++];
        assert(token.type == expectedType);
    };

    auto checkpoint = [&]() {
        stream.advance(index);
        index = 0;
    };

    checkpoint();
    checkNext(TokenType::LParen);
    checkNext(TokenType::Symbol);
    checkpoint();
    checkNext(TokenType::Symbol);
    checkNext(TokenType::Symbol);
    checkNext(TokenType::RParen);
    checkpoint();
    checkNext(TokenType::Semi);
    checkNext(TokenType::Finished);
    checkpoint();
    checkNext(TokenType::Finished);
    checkpoint();
    checkpoint();
}

void testParser()
{
    PrefixParselet symbolParselet = [](Parser &parser, Token token) {
        return std::make_unique<SymbolExpression>(token.content);
    };

    Parser parser(Lexer("1"));
    parser.add(TokenType::Symbol, 0, symbolParselet);

    ParserResult result = parser.parse(0);
    if (result.hasValue()) {
        std::cout << "Parse succceeded" << std::endl;
    }
    else {
        ParserError error = result.error();
        std::cout << "Parse failed: " << error.message << std::endl;
        std::cout << "  row: " << error.row << std::endl;
        std::cout << "  col: " << error.col << std::endl;
    }
}

int main(int argc, char **argv)
{
    testLexer();
    testTokenStream();
    testParser();

    std::system("pause");

    return 0;
}
