#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    using Std = Grammar::Std;
    using Nud = Grammar::Nud;
    using Led = Grammar::Led;

    ExprResult stringNud(Parser &parser, Token token)
    {
        return parser.create<StringExpr>(token.content);
    }

    ExprResult groupNud(Parser &parser, Token token)
    {
        ExprResult result = parser.parseExpression(0);
        if (result.hasError()) {
            return std::move(result);
        }

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected left paren");
        }

        return result.value();
    }

    ExprResult derefLed(Parser &parser, const Expr *left, Token token)
    {
        if (left->nodeType() != NodeType::IdentifierExpr) {
            return ParserError(token, "Expected an identifier (lhs)");
        }

        ExprResult result = parser.parseExpression(BindingPower::Call);
        if (result.hasError()) {
            return std::move(result);
        }

        const Expr *right = result.value();
        if (right->nodeType() != NodeType::IdentifierExpr) {
            return ParserError(token, "Expected an identifier (rhs)");
        }

        return parser.create<DerefExpr>(static_cast<const IdentifierExpr *>(left),
            static_cast<const IdentifierExpr *>(right));
    }

    ExprResult callLed(Parser &parser, const Expr *left, Token token)
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
                args.push_back(result.value());
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

        return parser.create<CallExpr>(static_cast<const IdentifierExpr *>(left), std::move(args));
    }

    StmtResult expressionStd(Parser &parser, Token token)
    {
        ExprResult result = parser.parseExpression(0);
        if (result.hasError()) {
            return result.error();
        }

        return parser.create<ExprStmt>(result.value());
    }

    StmtResult blockStd(Parser &parser, Token token)
    {
        StmtVec stmts;
        while (parser.currentToken().type != TokenType::RCBrace) {
            StmtResult result = parser.parseStatement();
            if (result.hasError()) {
                return result.error();
            }
            else {
                stmts.push_back(result.value());
            }
        }

        if (!parser.advanceToken(TokenType::RCBrace)) {
            return ParserError(parser.currentToken(), "Internal parser error matching '}'");
        }

        return parser.create<BlockStmt>(std::move(stmts));
    }

    StmtResult ifStd(Parser &parser, Token token)
    {
        ExprResult exprResult(ParserError(token, ""));
        StmtResult stmtResult(ParserError(token, ""));
        const Expr *condExpr = nullptr;
        const Stmt *thenStmt = nullptr;
        const Stmt *elseStmt = nullptr;

        if (!parser.advanceToken(TokenType::LParen)) {
            return ParserError(parser.currentToken(), "Expected LParen");
        }

        exprResult = parser.parseExpression(0);
        if (exprResult.hasError()) {
            return std::move(exprResult.error());
        }

        condExpr = exprResult.value();

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected RParen");
        }

        stmtResult = parser.parseStatement();
        if (stmtResult.hasError()) {
            return std::move(stmtResult);
        }

        thenStmt = stmtResult.value();
        if (thenStmt->nodeType() != NodeType::BlockStmt) {
            return ParserError(parser.currentToken(), "Expected block stmt");
        }

        if (parser.currentToken().type != TokenType::Else) {
            return parser.create<IfStmt>(condExpr, thenStmt);
        }

        parser.consumeToken();
        stmtResult = parser.parseStatement();
        if (stmtResult.hasError()) {
            return std::move(stmtResult);
        }

        elseStmt = stmtResult.value();
        if (elseStmt->nodeType() != NodeType::BlockStmt) {
            return ParserError(parser.currentToken(), "Expected block stmt");
        }

        return parser.create<IfStmt>(condExpr, thenStmt, elseStmt);
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
    stmt(TokenType::Fn, &fnStd);
    stmt(TokenType::If, &ifStd);
    stmt(TokenType::Return, &returnStd);

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

StmtResult Grammar::std(Parser &parser, Token token) const
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

ExprResult Grammar::nud(Parser &parser, Token token) const
{
    parser.consumeToken();
    return getRule(token.type).nud(parser, token);
}

ExprResult Grammar::led(Parser &parser, const Expr *expr, Token token) const
{
    parser.consumeToken();
    return getRule(token.type).led(parser, expr, token);
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
