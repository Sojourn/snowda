#include "snw.h"

using namespace Snowda;

void testLexer()
{
    {
        Lexer lexer("(a bb ccc)\n"
                    "(\"stuff\")\n"
                    "var a.type = bb;");
        Token token;
        while (lexer.next(token)) {
            std::cout << token << std::endl;
            if (token.type == TokenType::Error) {
                break;
            }
        }
    }
}


int main(int argc, char **argv)
{
    testLexer();

    return 0;
}
