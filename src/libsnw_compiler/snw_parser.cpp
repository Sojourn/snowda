#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ParserError::ParserError(Parser &parser, StringView message)
    : message(message)
    , row(parser.row())
    , col(parser.col())
{
}

Parser::Parser(Lexer &lexer)
    : stream_(lexer)
{
}

ParserResult Parser::parseExpression(int bp)
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

int Parser::row()
{
    return currentToken().row;
}

int Parser::col()
{
    return currentToken().col;
}

Token Parser::currentToken()
{
    return stream_[0];
}

Token Parser::next()
{
    return stream_[1];
}

Token Parser::consume()
{
    const Token token = currentToken();
    stream_.advance(1);
    return token;
}

void Parser::add(TokenType key, NullDelimitedFunc nud)
{
    Symbol &symbol = getSymbol(key);
    symbol.nud = nud;
}

void Parser::add(TokenType key, int bp, LeftDelimitedFunc led)
{
    Symbol &symbol = getSymbol(key);
    symbol.bp = bp;
    symbol.led = led;
}

Parser::Symbol &Parser::getSymbol(TokenType key)
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
