#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ParserError::ParserError(const Token &token)
    : msg_(token.content)
    , col_(token.col)
    , row_(token.row)
{
    assert(token.type == TokenType::Error);
}

ParserError::ParserError(const Token &token, const StringView &msg)
    : msg_(msg)
    , col_(token.col)
    , row_(token.row)
{
}

const StringView &ParserError::msg() const
{
    return msg_;
}

size_t ParserError::col() const
{
    return col_;
}

size_t ParserError::row() const
{
    return row_;
}
