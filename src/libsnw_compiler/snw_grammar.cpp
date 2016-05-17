#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
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

    // class NodeContentBuilder {
    // public:
    //     NodeContentBuilder(Parser &parser, Token token)
    //         : parser_(parser)
    //         , pos_(start.pos)
    //         , row_(start.row)
    //         , col_(start.col)
    //     {
    //     }

    //     NodeContent nodeContent()
    //     {
    //         NodeContent rval;
    //         rval.row = row_;
    //         rval.col = col_;
    //         rval.len = parser_.currentToken().pos - pos_;
    //         return rval;
    //     }

    // private:
    //     Parser &parser_;
    //     const size_t pos_;
    //     const size_t row_;
    //     const size_t col_;
    // };

    ParserResult defaultNud(Parser &parser, Token token)
    {
        return ParserError(token, "default nud");
    }

    ParserResult defaultLed(Parser &parser, NodePtr node, Token token)
    {
        return ParserError(token, "default led");
    }

    ParserResult identifierNud(Parser &parser, Token token)
    {
        return NodePtr(new IdentifierExpr(token.content));
    }

    ParserResult numberNud(Parser &parser, Token token)
    {
        // FIXME: Get an actual number hierarchy
        int number = 0;
        for (char c: token.content) {
            number *= 10;
            number += (c - '0');
        }

        return NodePtr(new NumberExpr(NodeContent(), number));
    }

    ParserResult characterNud(Parser &parser, Token token)
    {
        assert(token.content.size() == 1);
        char value = token.content[0];

        return NodePtr(new CharacterExpr(NodeContent(), value));
    }

    ParserResult stringNud(Parser &parser, Token token)
    {
        return NodePtr(new StringExpr(NodeContent(), token.content));
    }

    ParserResult groupNud(Parser &parser, Token token)
    {
        ParserResult result = parser.parseExpression(0);
        if (result.hasError()) {
            return std::move(result);
        }

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected left paren");
        }

        return std::move(result.value());
    }

    ParserResult derefLed(Parser &parser, NodePtr left, Token token)
    {
        if (dynamic_cast<const IdentifierExpr *>(left.get()) == nullptr) {
            return ParserError(token, "Expected an identifier (lhs)");
        }

        ParserResult result = parser.parseExpression(BindingPower::Call);
        if (result.hasError()) {
            return std::move(result);
        }

        NodePtr right = std::move(result.value());
        if (dynamic_cast<const IdentifierExpr *>(right.get()) == nullptr) {
            return ParserError(token, "Expected an identifier (rhs)");
        }

        return NodePtr(new DerefExpr(NodeContent(), std::move(left), std::move(right)));
    }

    ExprResult callLed(Parser &parser, NodePtr left, Token token)
    {
        if (left->nodeType() != NodeType::IdentifierExpr) {
            return ParserError(token, "Expected an identifier (lhs)");
        }

        ExprVec args;
        while (parser.currentToken().type != TokenType::RParen) {
            ExprResult result = parser.parseExpression(0);
            if (result.hasError()) {
                return std::move(result);
            }
            else {
                args.push_back(std::move(result.value()));
                if (parser.currentToken().type == TokenType::Comma) {
                    if (parser.nextToken().type == TokenType::RParen) {
                        return ParserError(parser.currentToken(), "Comma trailing argument list");
                    }
                    else {
                        parser.consumeToken();
                    }
                }
            }
        }

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Internal parser error matching ')'");
        }

        return NodePtr(new CallExpr(NodeContent(), std::move(identExpr), std::move(args)));
    }

    ParserResult expressionStd(Parser &parser, Token token)
    {
        ParserResult result = parser.parseExpression(0);
        if (result.hasError()) {
            return std::move(result);
        }

        if (!parser.advanceToken(TokenType::Semi)) {
            return ParserError(parser.currentToken(), "Expected statement expression to end with a ';'");
        }

        return NodePtr(new ExprStmt(NodeContent(), std::move(result.value())));
    }

    ParserResult blockStd(Parser &parser, Token token)
    {
        StmtVec stmts;
        while (parser.currentToken().type != TokenType::RCBrace) {
            ParserResult result = parser.parseStatement();
            if (result.hasError()) {
                return std::move(result);
            }
            else {
                stmts.push_back(std::move(result.value()));
            }
        }

        if (!parser.advanceToken(TokenType::RCBrace)) {
            return ParserError(parser.currentToken(), "Internal parser error matching '}'");
        }

        return NodePtr(new BlockStmt(NodeContent(), std::move(stmts)));
    }

    ParserResult ifStd(Parser &parser, Token token)
    {
		ParserResult result(ParserError(token, ""));
        NodePtr condExpr;
        NodePtr thenExpr;
        // std::vector<std::tuple(Expr, Expr)> elifExprs;
        NodePtr elseExpr;

        if (!parser.advanceToken(TokenType::LParen)) {
            return ParserError(parser.currentToken(), "Expected LParen");
        }

        result = parser.parseExpression(0);
        if (result.hasError()) {
            return std::move(result);
        }

        condExpr = std::move(result.value());

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected RParen");
        }

		result = parser.parseStatement();
        if (result.hasError()) {
            return std::move(result);
        }

        thenExpr = std::move(result.value());

        if (parser.currentToken().type != TokenType::Else) {
            return NodePtr(new IfStmt(NodeContent(), std::move(condExpr), std::move(thenExpr)));
        }

		parser.consumeToken();
		result = parser.parseStatement();
        if (result.hasError()) {
            return std::move(result);
        }

        elseExpr = std::move(result.value());
        return NodePtr(new IfStmt(NodeContent(), std::move(condExpr), std::move(thenExpr), std::move(elseExpr)));
    }

    ParserResult errorNud(Parser &parser, Token token)
    {
        return ParserError(token, token.content);
    }

    ParserResult errorLed(Parser &parser, NodePtr node, Token token)
    {
        return ParserError(token, token.content);
    }

    template<UnaryExpr::Operator op>
    ParserResult unaryNud(Parser &parser, Token token) {
        ParserResult result = parser.parseExpression(BindingPower::Unary);
        if (result.hasError()) {
            return result;
        }
        else {
            Expr expr = std::move(result.value());
            return NodeExpr(new UnaryExpr(NodeContent(), op, std::move(expr)));
        }
    }

    template<BinaryExpr::Operator op>
    ParserResult binaryLed(Parser &parser, NodePtr left, Token token) {
        ParserResult result = parser.parseExpression(parser.grammar().bp(token));
        if (result.hasError()) {
            return result;
        }
        else {
            Expr right = std::move(result.value());
            return NodePtr(new BinaryExpr(NodeContent(), op, std::move(left), std::move(right)));
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
    infix(TokenType::LParen, BindingPower::Call, &callLed);
    infix(TokenType::Dot, BindingPower::Call, &derefLed);

    stmt(TokenType::LCBrace, &blockStd);
    stmt(TokenType::If, &ifStd);

    prefix(TokenType::Plus, &unaryNud<UnaryExpr::Operator::Plus>);
    prefix(TokenType::Minus, &unaryNud<UnaryExpr::Operator::Minus>);
    prefix(TokenType::Tilde, &unaryNud<UnaryExpr::Operator::Tilde>);
    prefix(TokenType::Bang, &unaryNud<UnaryExpr::Operator::Bang>);

    infix(TokenType::Plus, BindingPower::Sum, &binaryLed<BinaryExpr::Operator::Add>);
    infix(TokenType::Minus, BindingPower::Sum, &binaryLed<BinaryExpr::Operator::Sub>);
    infix(TokenType::Mult, BindingPower::Product, &binaryLed<BinaryExpr::Operator::Mul>);
    infix(TokenType::Div, BindingPower::Product, &binaryLed<BinaryExpr::Operator::Div>);

    prefix(TokenType::Error, &errorNud);
    infix(TokenType::Error, BindingPower::None, &errorLed);
}

int Grammar::bp(Token token) const
{
    return getRule(token.type).bp;
}

ParserResult Grammar::std(Parser &parser, Token token) const
{
    const Rule &rule = getRule(token.type);
    if (rule.std) {
        parser.consumeToken();
        return rule.std(parser, token);
    }
    else {
        // The current token is part of the expression; don't consume
        return expressionStd(parser, token);
    }
}

ParserResult Grammar::nud(Parser &parser, Token token) const
{
    parser.consumeToken();
    return getRule(token.type).nud(parser, token);
}

ParserResult Grammar::led(Parser &parser, NodePtr node, Token token) const
{
    parser.consumeToken();
    return getRule(token.type).led(parser, std::move(node), token);
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

void Grammar::stmt(TokenType type, Std std)
{
    Rule &rule = getRule(type);
    rule.std = std;
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
    , std(nullptr)
    , nud(&defaultNud)
    , led(&defaultLed)
{
}
