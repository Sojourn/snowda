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
    checkNext(TokenType::Identifier);
    checkpoint();
    checkNext(TokenType::Identifier);
    checkNext(TokenType::Identifier);
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
	Lexer lexer(
        "{" \
        "    if (a + b) {" \
        "        print(\"stuff\");" \
        "    }" \
        "    else {}" \
        "    1 + 2 / (3 + 4);" \
        "}"
        );
    Parser parser(lexer);
    while (!parser.finished()) {
        StmtResult result = parser.parseStatement();
        if (result.hasValue()) {
            Ast::Printer printer;
            result.value()->visit(printer);
        }
        else {
            ParserError error = result.error();
            std::cout << "Parse failed: " << error.msg() << std::endl;
            std::cout << "  row: " << error.row() << std::endl;
            std::cout << "  col: " << error.col() << std::endl;
            break;
        }
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
