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

    namespace BindingPower {
        enum {
            None       = 0,
            Assignment = 10,
            Logical    = 20,
            Relational = 30,
            Sum        = 40,
            Product    = 50,
            Unary      = 60,
            Call       = 70,
        };
    }

    struct Symbol {
        int               bp;
        NullDelimitedFunc nud;
        LeftDelimitedFunc led;
    };

    class Parser {
        friend class ParserState;
    public:
        explicit Parser(Lexer &lexer)
            : stream_(lexer)
        {
        }

        ParserResult parseExpression(int bp)
        {
            Token token = consume();
            ParserResult result = getSymbol(token.type).nud(*this, token);
            if (result.hasError()) {
                return std::move(result);
            }
            else {
                Ast::ExpressionPtr expr = std::move(result.value());
                for (;;) {
                    token = consume();
                    const Symbol &symbol = getSymbol(token.type);
                    if (bp >= symbol.bp) {
                        return std::move(expr);
                    }
                    else {
                        result = symbol.led(*this, std::move(expr), token);
                        if (result.hasError()) {
                            return std::move(result);
                        }
                        else {
                            expr = std::move(result.value());
                        }
                    }
                }
            }
        }

        int row()
        {
            return currentToken().row;
        }

        int col()
        {
            return currentToken().col;
        }

        Token currentToken()
        {
            return stream_[0];
        }

        Token next()
        {
            return stream_[1];
        }

        Token consume()
        {
            const Token token = currentToken();
            stream_.advance(1);
            return token;
        }

        void add(TokenType key, NullDelimitedFunc nud)
        {
            Symbol &symbol = getSymbol(key);
            symbol.nud = nud;
        }

        void add(TokenType key, int bp, LeftDelimitedFunc led)
        {
            Symbol &symbol = getSymbol(key);
            symbol.bp = bp;
            symbol.led = led;
        }

    private:
        Symbol &getSymbol(TokenType key)
        {
            auto it = symbols_.find(key);
            if (it == symbols_.end()) {
                const NullDelimitedFunc defaultNud = [](Parser &parser, Token token) -> ParserResult {
                    return ParserError(parser, "No null delimited parselet");
                };
                const LeftDelimitedFunc defaultLed = [](Parser &parser, Ast::ExpressionPtr expr, Token token) -> ParserResult {
                    return ParserError(parser, "No left delimited parselet");
                };

                Symbol symbol;
                symbol.bp = BindingPower::None;
                symbol.nud = defaultNud;
                symbol.led = defaultLed;
                std::tie(it, std::ignore) = symbols_.insert(std::make_pair(key, symbol));
            }

            return it->second;
        }

    private:
        TokenStream stream_;
        std::map<TokenType, Symbol> symbols_;
    };

}

#endif // SNW_PARSER_H
