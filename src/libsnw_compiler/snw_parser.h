#ifndef SNW_PARSER_H
#define SNW_PARSER_H

namespace Snowda {

    class Parser {
    public:
        explicit Parser(Lexer &lexer);

        ParserResult parseExpression(int bp);

        int row();
        int col();

        Token currentToken();
        Token next();
        Token consume();
        bool advance(TokenType type);

    private:
        TokenStream stream_;
        Grammar grammar_;
    };

}

#endif // SNW_PARSER_H
