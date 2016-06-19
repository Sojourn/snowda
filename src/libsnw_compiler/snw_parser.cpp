#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    static const size_t maxParserDepth = 4096;
}

ParserFrame::ParserFrame(Parser &parser)
    : parser_(parser)
    , prev_(parser.frame_)
    // , pos_(parser.currentToken().pos)
	, pos_(0)
    , row_(parser.currentToken().row)
    , col_(parser.currentToken().col)
{
    parser_.depth_ += 1;
    parser_.frame_ = this;
}

ParserFrame::~ParserFrame()
{
    parser_.depth_ -= 1;
    parser_.frame_ = prev_;
}

NodeContent ParserFrame::nodeContent()
{
    Ast::NodeContent nodeContent;
    nodeContent.row = row_;
    nodeContent.col = col_;
    // content.len = parser_.currentToken().pos - pos_;
    nodeContent.len = 0;
    return nodeContent;
}

Parser::Parser(MemoryManager &memoryManager, Lexer &lexer)
    : memoryManager_(memoryManager)
    , stream_(lexer)
    , depth_(0)
    , frame_(nullptr)
{
}

bool Parser::finished()
{
    return currentToken().type == TokenType::Finished;
}

ExprResult Parser::parseExpression(int bp)
{
    ParserFrame frame(*this);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    Token token = currentToken();
    ExprResult result = grammar_.nud(*this, token);
    if (result.hasError()) {
        return std::move(result);
    }
    else {
        const Expr *expr = result.value();
        while (bp < grammar_.bp(currentToken())) {
            token = currentToken();
            result = grammar_.led(*this, expr, token);
            if (result.hasError()) {
                return std::move(result);
            }
            else {
				expr = result.value();
            }
        }

        return std::move(expr);
    }
}

StmtResult Parser::parseStatement()
{
    ParserFrame frame(*this);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    return grammar_.std(*this, currentToken());
}

RootResult Parser::parseRootStatement()
{
    assert(depth_ == 0);

    ParserFrame frame(*this);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    StmtVec stmts;
    while (!finished()) {
        StmtResult result = parseStatement();
        if (result.hasError()) {
            return result.error();
        }
        else {
            stmts.push_back(result.value());
        }
    }

    return create<RootStmt>(std::move(stmts));
}

size_t Parser::row()
{
    return currentToken().row;
}

size_t Parser::col()
{
    return currentToken().col;
}

Token Parser::currentToken()
{
    return stream_[0];
}

Token Parser::nextToken()
{
    return stream_[1];
}

Token Parser::consumeToken()
{
    const Token token = currentToken();
    stream_.advance(1);
    return token;
}

bool Parser::advanceToken(TokenType type)
{
    if (currentToken().type != type) {
        return false;
    }
    else {
        consumeToken();
        return true;
    }
}

const Grammar &Parser::grammar() const
{
    return grammar_;
}
