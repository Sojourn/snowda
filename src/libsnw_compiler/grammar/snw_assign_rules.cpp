#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::assignLed(Parser &parser, const Expr *left, Token token) {
    // TODO: Figure out an exhaustive list of what left can be
    // if (left->nodeType() != NodeType::IdentifierExpr) {
    //     return ParserError(parser.currentToken(), "Expected lhs to be an identifier");
    // }

    ExprResult result = parser.parseExpression(parser.grammar().bp(token));
    if (result.hasError()) {
        return result;
    }
    else {
        return parser.create<AssignExpr>(left, result.value());
    }
}
