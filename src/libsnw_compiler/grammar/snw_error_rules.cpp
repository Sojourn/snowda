#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::errorNud(Parser &parser, Token token)
{
    return ParserError(token, token.content);
}

ExprResult Snowda::errorLed(Parser &parser, const Expr *left, Token token)
{
    return ParserError(token, token.content);
}
