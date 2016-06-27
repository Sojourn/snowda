#ifndef SNW_AST_H
#define SNW_AST_H

#define SNW_AST_EXPR_TYPES \
    X(NumberExpr) \
    X(CharacterExpr) \
    X(StringExpr) \
    X(IdentifierExpr) \
    X(BinaryExpr) \
    X(UnaryExpr) \
    X(CallExpr) \
    X(DerefExpr) \
    X(AssignExpr)

#define SNW_AST_STMT_TYPES \
    X(RootStmt) \
    X(ModuleStmt) \
    X(BlockStmt) \
    X(FunctionArgStmt) \
    X(FunctionDeclStmt) \
    X(IfStmt) \
    X(ReturnStmt) \
    X(ForStmt) \
    X(ExprStmt)

#define SNW_AST_NODE_TYPES \
    SNW_AST_EXPR_TYPES \
    SNW_AST_STMT_TYPES

#define SNW_AST_UNARY_OPERATORS \
    X(Plus,  "+") \
    X(Minus, "-") \
    X(Tilde, "~") \
    X(Bang,  "!")

#define SNW_AST_BINARY_OPERATORS \
    X(Add,    "+") \
    X(Sub,    "-") \
    X(Mul,    "*") \
    X(Div,    "/") \
    X(Mod,    "%") \
    X(Or,     "|") \
    X(XOr,    "^") \
    X(And,    "&") \
    X(LShift, "<<") \
    X(RShift, ">>")

#include "snw_ast_node.h"
#include "snw_ast_expr.h"
#include "snw_ast_stmt.h"

#endif // SNW_AST_H
