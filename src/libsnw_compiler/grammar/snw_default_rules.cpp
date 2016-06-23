#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::defaultNud(Parser &parser, Token token)
{
    return ParserError(token, "default nud");
}

ExprResult Snowda::defaultLed(Parser &parser, const Expr *node, Token token)
{
    return ParserError(token, "default led");
}
