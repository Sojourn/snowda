#ifndef SNW_ERROR_RULES_H
#define SNW_ERROR_RULES_H

namespace Snowda {
    ExprResult errorNud(Parser &parser, Token token);
    ExprResult errorLed(Parser &parser, const Ast::Expr *left, Token token);
}

#endif // SNW_ERROR_RULES_H
