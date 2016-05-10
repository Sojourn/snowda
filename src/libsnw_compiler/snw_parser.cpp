#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Parser::Parser(Lexer &lexer)
    : stream_(lexer)
{
}

ParserResult Parser::parseExpression(int bp)
{
    Token token = consume();
    ParserResult result = grammar_.nud(*this, token);
    if (result.hasError()) {
        return std::move(result);
    }
    else {
        ExpressionPtr expr = std::move(result.value());
        for (;;) {
            token = consume();
            if (bp >= grammar_.bp(token)) {
                return std::move(expr);
            }
            else {
                result = grammar_.led(*this, std::move(expr), token);
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
