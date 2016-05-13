#ifndef SNW_PARSER_H
#define SNW_PARSER_H

namespace Snowda {

    class Parser {
    public:
        explicit Parser(Lexer &lexer);

        bool finished();
        ParserResult parseExpression(int bp);
        ParserResult parseStatement();

        int row();
        int col();

        Token currentToken();
		Token nextToken();
        Token consumeToken();
        bool advanceToken(TokenType type);

        const Grammar &grammar() const;

    private:
        TokenStream stream_;
        const Grammar grammar_;
    };

}

#endif // SNW_PARSER_H
