#include "snw_compiler.h"

using namespace Snowda;

StringView Snowda::name(TokenType type)
{
    switch(type) {
#define X(xType, xName) case TokenType::xType: return StringView(xName);
    SNW_TOKENS
#undef X
        default: abort();
    }
}

Token::Token()
    : type(TokenType::Error)
    , content("Uninitialized token")
    , row(0)
    , col(0)
{
}

std::ostream &Snowda::operator<<(std::ostream &os, const Token &token)
{
    os << "Token {\n";
    os << "  type:    " << name(token.type) << '\n';
    os << "  row:     " << token.row << '\n';
    os << "  col:     " << token.col << '\n';
    os << "  content: " << token.content << '\n';
    os << "}";

    return os;
}