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

BlockStmt::BlockStmt(NodeContent nodeContent, StmtVec stmts)
    : Stmt(NodeType::BlockStmt, nodeContent)
    , stmts_(std::move(stmts))
{
}

const StmtVec &BlockStmt::stmts() const
{
    return stmts_;
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

FunctionArgStmt::FunctionArgStmt(NodeContent nodeContent, StringView name, StringView type)
    : Stmt(NodeType::FunctionArgStmt, nodeContent)
    , name_(name)
    , type_(type)
{
}

const StringView &FunctionArgStmt::name() const
{
    return name_;
}

const StringView &FunctionArgStmt::type() const
{
    return type_;
}

FunctionDeclStmt::FunctionDeclStmt(NodeContent nodeContent, StringView name, FunctionArgStmtVec args, const BlockStmt *block)
    : Stmt(NodeType::FunctionDeclStmt, nodeContent)
    , name_(name)
    , args_(std::move(args))
    , block_(block)
{
}

const StringView &FunctionDeclStmt::name() const
{
    return name_;
}

const FunctionArgStmtVec &FunctionDeclStmt::args() const
{
    return args_;
}

const BlockStmt *FunctionDeclStmt::block() const
{
    return block_;
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

ReturnStmt::ReturnStmt(NodeContent nodeContent, const ExprStmt *exprStmt)
    : Stmt(NodeType::ReturnStmt, nodeContent)
    , exprStmt_(exprStmt)
{
}

const ExprStmt *ReturnStmt::exprStmt() const
{
    return exprStmt_;
}

ForStmt::ForStmt(NodeContent nodeContent)
    : Stmt(NodeType::ForStmt, nodeContent)
{
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
