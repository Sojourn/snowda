#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::characterNud(Parser &parser, Token token)
{
    // FIXME: Multi-byte character class
    return parser.create<CharacterExpr>(token.content[0]);
}
