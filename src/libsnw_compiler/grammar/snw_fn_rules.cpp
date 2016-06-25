#include "../snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    using FnArgsResult = Result<FunctionArgStmtVec, ParserError>;

    FnArgsResult parseFnArgs(Parser &parser)
    {
        if (!parser.advanceToken(TokenType::LParen)) {
            return ParserError(parser.currentToken(), "Expected LParen");
        }

        FunctionArgStmtVec args;
        while (parser.currentToken().type != TokenType::RParen) {
            Token token = parser.consumeToken();
            if (token.type != TokenType::Identifier) {
                return ParserError(token, "Expected Identifier");
            }
            StringView argName = token.content;

            if (!parser.advanceToken(TokenType::Colon)) {
                return ParserError(parser.currentToken(), "Expected Colon");
            }

            token = parser.consumeToken();
            if (token.type != TokenType::Identifier) {
                return ParserError(token, "Expected Identifier");
            }
            StringView argType = token.content;

            args.push_back(parser.create<FunctionArgStmt>(argName, argType));

            if (parser.currentToken().type == TokenType::Comma && parser.nextToken().type == TokenType::Identifier) {
                parser.consumeToken();
            }
        }

        if (!parser.advanceToken(TokenType::RParen)) {
            return ParserError(parser.currentToken(), "Expected RParen");
        }

        return std::move(args);
    }
}

StmtResult Snowda::fnStd(Parser &parser, Token token)
{
    token = parser.consumeToken();
    if (token.type != TokenType::Identifier) {
        return ParserError(token, "Expected Identifier");
    }
    StringView name = token.content;

    FnArgsResult argsResult = parseFnArgs(parser);
    if (argsResult.hasError()) {
        return argsResult.error();
    }
    FunctionArgStmtVec args = std::move(argsResult.value());

    if (parser.currentToken().type != TokenType::LCBrace) {
        return ParserError(parser.currentToken(), "Expected LCBrace");
    }

    StmtResult result = parser.parseStatement();
    if (result.hasError()) {
        return std::move(result.error());
    }

    const Stmt *stmt = result.value();
    if (stmt->nodeType() != NodeType::BlockStmt) {
        return ParserError(parser.currentToken(), "Expected BlockStmt");
    }

    const BlockStmt *blockStmt = static_cast<const BlockStmt *>(stmt);
    return parser.create<FunctionDeclStmt>(name, std::move(args), blockStmt);
}
