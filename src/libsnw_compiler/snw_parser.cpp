#include "snw_compiler.h"

using namespace Snowda;
using namespace Snowda::Ast;

namespace {
    static const size_t maxParserDepth = 4096;

    class ParserFrame {
    public:
        ParserFrame(size_t &depth)
            : depth_(depth)
        {
            depth_ += 1;
        }

        ~ParserFrame()
        {
            depth_ -= 1;
        }

    private:
        size_t &depth_;
    };
}

Parser::Parser(Lexer &lexer)
    : stream_(lexer)
    , depth_(0)
{
}

bool Parser::finished()
{
    return currentToken().type == TokenType::Finished;
}

ParserResult Parser::parseExpression(int bp)
{
    ParserFrame frame(depth_);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    Token token = currentToken();
    ParserResult result = grammar_.nud(*this, token);
    if (result.hasError()) {
        return std::move(result);
    }
    else {
        NodePtr expr = std::move(result.value());
        while (bp < grammar_.bp(currentToken())) {
            token = currentToken();
            result = grammar_.led(*this, std::move(expr), token);
            if (result.hasError()) {
                return std::move(result);
            }
            else {
                expr = std::move(result.value());
            }
        }

        return std::move(expr);
    }
}

ParserResult Parser::parseStatement()
{
    ParserFrame frame(depth_);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    return grammar_.std(*this, currentToken());
}

ParserResult Parser::parseRootStatement()
{
    assert(depth_ == 0);

    ParserFrame frame(depth_);
    if (depth_ >= maxParserDepth) {
        return ParserError(currentToken(), "Max parse depth reached");
    }

    StmtVec stmts;
    while (!finished()) {
        ParserResult result = parseStatement();
        if (result.hasError()) {
            return result;
        }
        else {
            stmts.push_back(std::move(result.value()));
        }
    }

    return NodePtr(new RootStmt(NodeContent(), std::move(stmts)));
}

int Parser::row()
{
    return currentToken().row;
}

int Parser::col()
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
