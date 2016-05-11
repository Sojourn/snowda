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
        // FIXME: Get an actual number hierarchy
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

    ParserResult groupNud(Parser &parser, Token token)
    {
        ParserResult result = parser.parseExpression(0);
        if (result.hasError()) {
            return std::move(result);
        }

        if (!parser.advance(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected left paren");
        }

        return std::move(result.value());
    }

    ParserResult derefLed(Parser &parser, Expr left, Token token)
    {
        if (dynamic_cast<const IdentifierExpression *>(left.get()) == nullptr) {
            return ParserError(token, "Expected an identifier (lhs)");
        }

        ParserResult result = parser.parseExpression(BindingPower::Call);
        if (result.hasError()) {
            return std::move(result);
        }

        Expr right = std::move(result.value());
        if (dynamic_cast<const IdentifierExpression *>(right.get()) == nullptr) {
            return ParserError(token, "Expected an identifier (rhs)");
        }

        return Expr(new DerefExpression(std::move(left), std::move(right)));
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
    ParserResult unaryNud(Parser &parser, Token token) {
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
    ParserResult binaryLed(Parser &parser, Expr left, Token token) {
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
    prefix(TokenType::Identifier, &identifierNud);
    prefix(TokenType::Number, &numberNud);
    prefix(TokenType::Character, &characterNud);
    prefix(TokenType::String, &stringNud);

    prefix(TokenType::LParen, &groupNud);
    infix(TokenType::Dot, BindingPower::Call, &derefLed);

    prefix(TokenType::Plus, &unaryNud<UnaryOperator::Plus>);
    prefix(TokenType::Minus, &unaryNud<UnaryOperator::Minus>);
    prefix(TokenType::Tilde, &unaryNud<UnaryOperator::Tilde>);
    prefix(TokenType::Bang, &unaryNud<UnaryOperator::Bang>);

    infix(TokenType::Plus, BindingPower::Sum, &binaryLed<BinaryOperator::Add, BindingPower::Sum>);
    infix(TokenType::Minus, BindingPower::Sum, &binaryLed<BinaryOperator::Sub, BindingPower::Sum>);
    infix(TokenType::Mult, BindingPower::Product, &binaryLed<BinaryOperator::Mul, BindingPower::Product>);
    infix(TokenType::Div, BindingPower::Product, &binaryLed<BinaryOperator::Div, BindingPower::Product>);

    prefix(TokenType::Error, &errorNud);
    infix(TokenType::Error, BindingPower::None, &errorLed);
}

int Grammar::bp(Token token) const
{
    return getRule(token.type).bp;
}

ParserResult Grammar::nud(Parser &parser, Token token) const
{
    return getRule(token.type).nud(parser, token);
}

ParserResult Grammar::led(Parser &parser, Expr expr, Token token) const
{
    return getRule(token.type).led(parser, std::move(expr), token);
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

void Grammar::prefix(TokenType type, Nud nud)
{
    Rule &rule = getRule(type);
    rule.nud = nud;
}

void Grammar::infix(TokenType type, int bp, Led led)
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
