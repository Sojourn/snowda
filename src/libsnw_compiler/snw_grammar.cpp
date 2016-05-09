#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    using Expr = Grammar::Expr;
    using Nud = Grammar::Nud;
    using Led = Grammar::Led;

    ParserResult defaultNud(Parser &parser, Token token)
    {
        return ParserError(token, "default nud");
    }

    ParserResult defaultLed(Parser &parser, Expr expr, Token token)
    {
        return ParserError(token, "default led");
    }

    ParserResult errorNud(Parser &parser, Token token)
    {
        return ParserError(token, token.content);
    }

    ParserResult errorLed(Parser &parser, Expr expr, Token token)
    {
        return ParserError(token, token.content);
    }
}

Grammar::Grammar()
{
    addRule(TokenType::Error, &errorNud);
    addRule(TokenType::Error, 0, &errorLed);
}

ParserResult Grammar::nud(TokenType type, Parser &parser, Token token) const
{
    return getRule(type).nud(parser, token);
}

ParserResult Grammar::led(TokenType type, Parser &parser, Expr expr, Token token) const
{
    return getRule(type).led(parser, std::move(expr), token);
}

Grammar::Rule &Grammar::getRule(TokenType type)
{
    assert(type != TokenType::Count);
    return rules_[static_cast<size_t>(type)];
}

const Grammar::Rule &Grammar::getRule(TokenType type) const
{
    assert(type != TokenType::Count);
    return rules_[static_cast<size_t>(type)];
}

void Grammar::addRule(TokenType type, Nud nud)
{
    Rule &rule = getRule(type);
    rule.nud = nud;
}

void Grammar::addRule(TokenType type, int bp, Led led)
{
    Rule &rule = getRule(type);
    rule.bp = bp;
    rule.led = led;
}

Grammar::Rule::Rule()
    : bp(0)
    , nud(&defaultNud)
    , led(&defaultLed)
{
}
