#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

StringView Snowda::Ast::str(UnaryOperator op)
{
    switch (op) {
#define X(xType, xName) \
    case UnaryOperator::xType: \
        return StringView(xName);
        SNW_AST_UNARY_OPERATORS
#undef X
    default:
        abort();
    }
}

StringView Snowda::Ast::str(BinaryOperator op)
{
    switch (op) {
#define X(xType, xName) \
    case BinaryOperator::xType: \
        return StringView(xName);
        SNW_AST_BINARY_OPERATORS
#undef X
    default:
        abort();
    }
}

IdentifierExpression::IdentifierExpression(StringView name)
    : name_(name)
{
}

StringView IdentifierExpression::name() const
{
    return name_;
}

void IdentifierExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}


DerefExpression::DerefExpression(ExpressionPtr parent, ExpressionPtr child)
    : IdentifierExpression(dynamic_cast<const IdentifierExpression *>(child.get())->name())
    , parent_(std::move(parent))
    , child_(std::move(child))
{
}

const ExpressionPtr &DerefExpression::parent() const
{
    return parent_;
}

const ExpressionPtr &DerefExpression::child() const
{
    return child_;
}

void DerefExpression::accept(Visitor &visitor) const
{
    return visitor.visit(*this);
}

CallExpression::CallExpression(ExpressionPtr ident, std::vector<ExpressionPtr> args)
    : ident_(std::move(ident))
    , args_(std::move(args))
{
}

const ExpressionPtr &CallExpression::ident() const
{
    return ident_;
}

const std::vector<ExpressionPtr> &CallExpression::args() const
{
    return args_;
}

void CallExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

ConditionalExpression::ConditionalExpression(ExpressionPtr condExpr, ExpressionPtr thenExpr)
    : cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
{
    assert(cond_);
    assert(then_);
}

ConditionalExpression   ::ConditionalExpression(ExpressionPtr condExpr, ExpressionPtr thenExpr, ExpressionPtr elseExpr)
    : cond_(std::move(condExpr))
    , then_(std::move(thenExpr))
    , else_(std::move(elseExpr))
{
    assert(cond_);
    assert(then_);
    assert(else_);
}

const ExpressionPtr &ConditionalExpression::condExpr() const
{
    return cond_;
}

const ExpressionPtr &ConditionalExpression::thenExpr() const
{
    return then_;
}

const ExpressionPtr &ConditionalExpression::elseExpr() const
{
    return else_;
}

void ConditionalExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

UnaryExpression::UnaryExpression(UnaryOperator op, ExpressionPtr expr)
    : op_(op)
    , expr_(std::move(expr))
{
    assert(expr_);
}

UnaryOperator UnaryExpression::op() const
{
    return op_;
}

const ExpressionPtr &UnaryExpression::expr() const
{
    return expr_;
}

void UnaryExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

BinaryExpression::BinaryExpression(BinaryOperator op, ExpressionPtr lhs, ExpressionPtr rhs)
    : op_(op)
    , lhs_(std::move(lhs))
    , rhs_(std::move(rhs))
{
    assert(lhs_);
    assert(rhs_);
}

BinaryOperator BinaryExpression::op() const
{
    return op_;
}

const ExpressionPtr &BinaryExpression::lhs() const
{
    return lhs_;
}

const ExpressionPtr &BinaryExpression::rhs() const
{
    return rhs_;
}

void BinaryExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

NumberExpression::NumberExpression(int value)
    : value_(value)
{
}

int NumberExpression::value() const
{
    return value_;
}

void NumberExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

CharacterExpression::CharacterExpression(char value)
    : value_(value)
{
}

char CharacterExpression::value() const
{
    return value_;
}

void CharacterExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

StringExpression::StringExpression(StringView value)
    : value_(value)
{
}

const StringView &StringExpression::value() const
{
    return value_;
}

void StringExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}
