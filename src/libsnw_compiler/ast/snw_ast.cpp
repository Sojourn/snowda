#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;


CompoundStatement::CompoundStatement(std::vector<StatementPtr> stmts)
    : stmts_(std::move(stmts))
{
}

const std::vector<StatementPtr> &CompoundStatement::stmts() const
{
    return stmts_;
}

void CompoundStatement::accept(Visitor &visitor) const
{
    visitor.visit(*this);
    for (auto &stmt: stmts_) {
        stmt->accept(visitor);
    }
}

SymbolExpression::SymbolExpression(StringView name)
    : name_(name)
{
}

StringView SymbolExpression::name() const
{
    return name_;
}

void SymbolExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}

ConditionalExpression::ConditionalExpression(StatementPtr cond, StatementPtr taken)
    : cond_(std::move(cond))
    , taken_(std::move(taken))
{
    assert(cond_);
    assert(taken_);
}

ConditionalExpression::ConditionalExpression(StatementPtr cond, StatementPtr taken, StatementPtr notTaken)
    : cond_(std::move(cond))
    , taken_(std::move(taken))
    , notTaken_(std::move(notTaken))
{
    assert(cond_);
    assert(taken_);
    assert(notTaken_);
}

void ConditionalExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
    cond_->accept(visitor);
    taken_->accept(visitor);
    if (notTaken_) {
        notTaken_->accept(visitor);
    }
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
    expr_->accept(visitor);
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
    lhs_->accept(visitor);
    rhs_->accept(visitor);
}

LiteralExpression::LiteralExpression(int value)
    : value_(value)
{
}

int LiteralExpression::value() const
{
    return value_;
}

void LiteralExpression::accept(Visitor &visitor) const
{
    visitor.visit(*this);
}
