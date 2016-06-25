#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

StmtResult Snowda::returnStd(Parser &parser, Token token)
{
    StmtResult result = parser.parseStatement();
    if (result.hasError()) {
        return result.error();
    }

    const Stmt *stmt = result.value();
    if (stmt->nodeType() != NodeType::ExprStmt) {
        return ParserError(parser.currentToken(), "Expected ExprStmt");
    }
    const ExprStmt *exprStmt = static_cast<const ExprStmt *>(stmt);
    return parser.create<ReturnStmt>(exprStmt);
}
