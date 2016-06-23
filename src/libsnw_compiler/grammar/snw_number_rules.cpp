#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

ExprResult Snowda::numberNud(Parser &parser, Token token)
{
    // FIXME: Get an actual number hierarchy
    int number = 0;
    for (char c: token.content) {
        number *= 10;
        number += (c - '0');
    }

    return parser.create<NumberExpr>(number);
}
