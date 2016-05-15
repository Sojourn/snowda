#ifndef SNW_AST_H
#define SNW_AST_H

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
