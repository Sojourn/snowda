#ifndef SNW_OPERATOR_RULES_H
#define SNW_OPERATOR_RULES_H

namespace Snowda {
    template<Ast::UnaryExpr::Operator op>
    ExprResult unaryNud(Parser &parser, Token token) {
        ExprResult result = parser.parseExpression(BindingPower::Unary);
        if (result.hasError()) {
            return result;
        }
        else {
            return parser.create<Ast::UnaryExpr>(op, result.value());
        }
    }

    template<Ast::BinaryExpr::Operator op>
    ExprResult binaryLed(Parser &parser, const Ast::Expr *left, Token token) {
        ExprResult result = parser.parseExpression(parser.grammar().bp(token));
        if (result.hasError()) {
            return result;
        }
        else {
            return parser.create<Ast::BinaryExpr>(op, left, result.value());
        }
    }
}

#endif // SNW_OPERATOR_RULES_H
