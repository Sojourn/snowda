#ifndef SNW_PARSER_H
#define SNW_PARSER_H

namespace Snowda {

    struct ParserError {
        StringView message;
        size_t     row;
        size_t     col;

        ParserError(const TokenStream &stream, StringView message);
    };
    using ParserResult = Result<Ast::ExpressionPtr, ParserError>;

    class Parser;

    using PrefixParselet = std::function<ParserResult(Parser &, Token)>;
    using InfixParselet = std::function<ParserResult(Parser &, Ast::ExpressionPtr, Token)>;

    class Parser {
        friend class ParserState;
    public:
        explicit Parser(Lexer lexer);

        ParserResult parse(int precedence);

        void add(TokenType key, int precedence, PrefixParselet parselet);
        void add(TokenType key, int precedence, InfixParselet parselet);

	private:
		int getPrecedence();

    private:
		using PrefixParseletMap = std::map<TokenType, std::tuple<int, PrefixParselet>>;
		using InfixParseletMap = std::map<TokenType, std::tuple<int, InfixParselet>>;

        TokenStream stream_;
        PrefixParseletMap prefixParselets_;
        InfixParseletMap infixParselets_;
    };

}

#endif // SNW_PARSER_H
