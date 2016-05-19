#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

Stmt::Stmt(NodeType nodeType, NodeContent nodeContent)
    : Node(nodeType, nodeContent)
{
}

RootStmt::RootStmt(NodeContent nodeContent, StmtVec stmts)
    : Stmt(NodeType::RootStmt, nodeContent)
    , stmts_(std::move(stmts))
{
}

const StmtVec &RootStmt::stmts() const
{
    return stmts_;
}

void RootStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

BlockStmt::BlockStmt(NodeContent nodeContent, StmtVec stmts)
    : Stmt(NodeType::BlockStmt, nodeContent)
    , stmts_(std::move(stmts))
{
}

const StmtVec &BlockStmt::stmts() const
{
    return stmts_;
}

void BlockStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

ModuleStmt::ModuleStmt(NodeContent nodeContent, StringView name, ExprVec args, const BlockStmt *block)
    : Stmt(NodeType::ModuleStmt, nodeContent)
    , name_(name)
    , args_(std::move(args))
    , block_(block)
{
}

const StringView &ModuleStmt::name() const
{
    return name_;
}

const ExprVec &ModuleStmt::args() const
{
    return args_;
}

const BlockStmt *ModuleStmt::block() const
{
    return block_;
}

void ModuleStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

DeclStmt::DeclStmt(NodeContent nodeContent, StringView name, const Expr *expr)
    : Stmt(NodeType::DeclStmt, nodeContent)
    , name_(name)
    , expr_(expr)
{
}

const StringView &DeclStmt::name() const
{
    return name_;
}

const Expr *DeclStmt::expr() const
{
    return expr_;
}

void DeclStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

IfStmt::IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(condExpr)
    , then_(thenStmt)
{
}

IfStmt::IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, ElifVec elifs)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(condExpr)
    , then_(thenStmt)
    , elifs_(std::move(elifs))
{
}

IfStmt::IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, const Stmt *elseStmt)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(condExpr)
    , then_(thenStmt)
    , else_(elseStmt)
{
}

IfStmt::IfStmt(NodeContent nodeContent, const Expr *condExpr, const Stmt *thenStmt, ElifVec elifs, const Stmt *elseStmt)
    : Stmt(NodeType::IfStmt, nodeContent)
    , cond_(condExpr)
    , then_(thenStmt)
    , elifs_(std::move(elifs))
    , else_(elseStmt)
{
}

const Expr *IfStmt::condExpr() const
{
    return cond_;
}

const Stmt *IfStmt::thenStmt() const
{
    return then_;
}

const IfStmt::ElifVec &IfStmt::elifs() const
{
    return elifs_;
}

const Stmt *IfStmt::elseStmt() const
{
    return else_;
}

void IfStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

ForStmt::ForStmt(NodeContent nodeContent)
    : Stmt(NodeType::ForStmt, nodeContent)
{
}

void ForStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}

ExprStmt::ExprStmt(NodeContent nodeContent, const Expr *expr)
    : Stmt(NodeType::ExprStmt, nodeContent)
    , expr_(expr)
{
}

const Expr *ExprStmt::expr() const
{
    return expr_;
}

void ExprStmt::visit(NodeVisitor &visitor) const
{
    visitor.visit(*this);
}
