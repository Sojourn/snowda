#ifndef SNWC_AST_DISPATCHER_H
#define SNWC_AST_DISPATCHER_H

template<typename Result, typename Visitor>
Result dispatch(Visitor &visitor, AstExpr *expr) {
    switch (expr->exprKind) {
    case AstExprKind::UnaryOperation:
        return visitor.visit(static_cast<UnaryOperationAstExpr *>(expr));
    case AstExprKind::BinaryOperation:
        return visitor.visit(static_cast<BinaryOperationAstExpr *>(expr));
    case AstExprKind::Assign:
        return visitor.visit(static_cast<AssignAstExpr *>(expr));
    case AstExprKind::Number:
        return visitor.visit(static_cast<NumberAstExpr *>(expr));
    case AstExprKind::Symbol:
        return visitor.visit(static_cast<SymbolAstExpr *>(expr));
    default:
        abort();
    }
}

#endif // SNWC_AST_DISPATCHER_H
