#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    using Expr = Grammar::Expr;
    using Nud = Grammar::Nud;
    using Led = Grammar::Led;

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

    ParserResult defaultNud(Parser &parser, Token token)
    {
        return ParserError(token, "default nud");
    }

    ParserResult defaultLed(Parser &parser, Expr expr, Token token)
    {
        return ParserError(token, "default led");
    }

    ParserResult identifierNud(Parser &parser, Token token)
    {
        return Expr(new IdentifierExpression(token.content));
    }

    ParserResult numberNud(Parser &parser, Token token)
    {
        int number = 0;
        for (char c: token.content) {
            number *= 10;
            number += (c - '0');
        }

        return Expr(new NumberExpression(number));
    }

    ParserResult characterNud(Parser &parser, Token token)
    {
        assert(token.content.size() == 1);
        char value = token.content[0];

        return Expr(new CharacterExpression(value));
    }

    ParserResult stringNud(Parser &parser, Token token)
    {
        return Expr(new StringExpression(token.content));
    }

    ParserResult errorNud(Parser &parser, Token token)
    {
        return ParserError(token, token.content);
    }

    ParserResult errorLed(Parser &parser, Expr expr, Token token)
    {
        return ParserError(token, token.content);
    }

    template<UnaryOperator op>
    ParserResult prefix (Parser &parser, Token token) {
        ParserResult result = parser.parseExpression(BindingPower::Unary);
        if (result.hasError()) {
            return result;
        }
        else {
            Expr expr = std::move(result.value());
            return Expr(new UnaryExpression(op, std::move(expr)));
        }
    }

    template<BinaryOperator op, int bp>
    ParserResult infix(Parser &parser, Expr left, Token token) {
        ParserResult result = parser.parseExpression(bp);
        if (result.hasError()) {
            return result;
        }
        else {
            Expr right = std::move(result.value());
            return Expr(new BinaryExpression(op, std::move(left), std::move(right)));
        }
    }
}

Grammar::Grammar()
{
    addRule(TokenType::Identifier, &identifierNud);
    addRule(TokenType::Number, &numberNud);
    addRule(TokenType::Character, &characterNud);
    addRule(TokenType::String, &stringNud);

    addRule(TokenType::Plus, &prefix<UnaryOperator::Plus>);
    addRule(TokenType::Plus, BindingPower::Sum, &infix<BinaryOperator::Add, BindingPower::Sum>);

    addRule(TokenType::Minus, &prefix<UnaryOperator::Minus>);
    addRule(TokenType::Minus, BindingPower::Sum, &infix<BinaryOperator::Sub, BindingPower::Sum>);

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
