#ifndef SNWC_AST_H
#define SNWC_AST_H

enum class AstExprKind {
    UnaryOperation,
    BinaryOperation,
    Assign,
    Number,
    Symbol,
};

struct AstExpr {
    AstExprKind exprKind;
};

enum class UnaryOperationKind {
    Neg,
    Pos,
};

struct UnaryOperationAstExpr : public AstExpr {
    UnaryOperationKind opKind;
    AstExpr *expr;
};

enum class BinaryOperationKind  {
    Add,
    Subtract,
};

struct BinaryOperationAstExpr : public AstExpr {
    BinaryOperationKind opKind;
    AstExpr *leftExpr;
    AstExpr *rightExpr;
};

struct AssignAstExpr : public AstExpr {
    AstExpr *rightExpr;
    AstExpr *leftExpr;
};

struct NumberAstExpr : public AstExpr {
    size_t pos;
    size_t len;
};

struct SymbolAstExpr : public AstExpr {
    size_t pos;
    size_t len;
};

#endif // SNWC_AST_H
