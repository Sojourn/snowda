#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::identifierNud(Parser &parser, Token token)
{
    return parser.create<IdentifierExpr>(token.content);
}
