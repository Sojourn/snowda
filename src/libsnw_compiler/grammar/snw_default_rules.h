#ifndef SNW_DEFAULT_RULES_H
#define SNW_DEFAULT_RULES_H

namespace Snowda {
    ExprResult defaultNud(Parser &parser, Token token);
    ExprResult defaultLed(Parser &parser, const Ast::Expr *node, Token token);
}

#endif // SNW_DEFAULT_RULES_H
