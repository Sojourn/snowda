#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ParserError::ParserError(const TokenStream &stream, StringView message)
    : message(message)
    , row(stream.row())
    , col(stream.col())
{
}

Parser::Parser(Lexer lexer)
    : stream_(lexer)
{
}

ParserResult Parser::parse(int precedence)
{
    Token token = stream_[0];
    stream_.advance(1);
    if (token.type == TokenType::Finished) {
        return ParserError(stream_, "That's all she wrote");
    }

    auto prefixIt = prefixParselets_.find(token.type);
    if (prefixIt == prefixParselets_.end()) {
        return ParserError(stream_, "Couldn't parse token as prefix");
    }

    auto &prefixEntry = std::get<1>(*prefixIt);
    auto &prefixParselet = std::get<1>(prefixEntry);
    ParserResult lhsResult = prefixParselet(*this, token);
    if (lhsResult.hasError()) {
        return std::move(lhsResult);
    }

    ExpressionPtr lhs = std::move(lhsResult.value());
    while (precedence < getPrecedence()) {
        Token token = stream_[0];
        stream_.advance(1);
        if (token.type == TokenType::Finished) {
            return ParserError(stream_, "That's all she wrote again");
        }

        auto infixIt = infixParselets_.find(token.type);
        assert(infixIt != infixParselets_.end());

        auto &infixEntry = std::get<1>(*infixIt);
        auto &infixParselet = std::get<1>(infixEntry);
        lhsResult = infixParselet(*this, std::move(lhs), token);
        if (lhsResult.hasError()) {
            return std::move(lhsResult);
        }

        lhs = std::move(lhsResult.value());
    }

    return std::move(lhs);
}

int Parser::getPrecedence()
{
    auto infixIt = infixParselets_.find(stream_[0].type);
    if (infixIt != infixParselets_.end()) {
        auto &entry = std::get<1>(*infixIt);
        return std::get<0>(entry);
    }
    else {
        return 0;
    }
}

void Parser::add(TokenType key, int precedence, PrefixParselet parselet)
{
    assert(prefixParselets_.count(key) == 0);

    prefixParselets_.insert(std::make_pair(key, std::make_tuple(precedence, parselet)));
}

void Parser::add(TokenType key, int precedence, InfixParselet parselet)
{
    assert(infixParselets_.count(key) == 0);

    infixParselets_.insert(std::make_pair(key, std::make_tuple(precedence, parselet)));
}
