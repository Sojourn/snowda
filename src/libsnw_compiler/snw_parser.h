#ifndef SNW_PARSER_H
#define SNW_PARSER_H

namespace Snowda {

    class Parser;
    class Parselet;

    struct ParserError {
        StringView message;
        size_t     row;
        size_t     col;

        ParserError(Parser &parser, StringView message);
    };
    
    using ParserResult = Result<Ast::ExpressionPtr, ParserError>;
    using NullDelimitedFunc = ParserResult (*)(Parser &, Token);
    using LeftDelimitedFunc = ParserResult (*)(Parser &, Ast::ExpressionPtr, Token);

    class Parser {
        friend class ParserState;
    public:
        explicit Parser(Lexer &lexer);

        ParserResult parseExpression(int bp);

        int row();
        int col();

        Token currentToken();
        Token next();
        Token consume();

        void add(TokenType key, NullDelimitedFunc nud);
        void add(TokenType key, int bp, LeftDelimitedFunc led);

    private:
        struct Symbol {
            int               bp;
            NullDelimitedFunc nud;
            LeftDelimitedFunc led;
        };

        Symbol &getSymbol(TokenType key);

    private:
        TokenStream stream_;
        std::map<TokenType, Symbol> symbols_;
    };

}

#endif // SNW_PARSER_H
