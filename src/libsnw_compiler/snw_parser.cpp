#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Parser::Parser(Lexer &lexer)
    : stream_(lexer)
{
}

bool Parser::finished()
{
    return currentToken().type == TokenType::Finished;
}

ParserResult Parser::parseExpression(int bp)
{
    Token token = currentToken();
    ParserResult result = grammar_.nud(*this, token);
    if (result.hasError()) {
        return std::move(result);
    }
    else {
        ExpressionPtr expr = std::move(result.value());
        while (bp < grammar_.bp(currentToken())) {
            token = currentToken();
            result = grammar_.led(*this, std::move(expr), token);
            if (result.hasError()) {
                return std::move(result);
            }
            else {
                expr = std::move(result.value());
            }
        }

        return std::move(expr);
    }
}

ParserResult Parser::parseStatement()
{
    return grammar_.std(*this, currentToken());
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

Token Parser::nextToken()
{
    return stream_[1];
}

Token Parser::consumeToken()
{
    const Token token = currentToken();
    stream_.advance(1);
    return token;
}

bool Parser::advanceToken(TokenType type)
{
    if (currentToken().type != type) {
        return false;
    }
    else {
        consumeToken();
        return true;
    }
}

const Grammar &Parser::grammar() const
{
    return grammar_;
}
