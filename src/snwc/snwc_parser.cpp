#include "snwc.h"

Parser::Parser(const char *src)
    : lexer_(src)
{
}

Lexer &Parser::lexer() {
    return lexer_;
}

const Lexer &Parser::lexer() const {
    return lexer_;
}

Grammar &Parser::grammar() {
    return grammar_;
}

const Grammar &Parser::grammar() const {
    return grammar_;
}

AstExpr *Parser::parseStmt() {
    bool advanced = false;
    do {
        Token token = lexer_.current();
        switch (token.kind) {
        case TokenKind::Newline:
        case TokenKind::Begin:
            lexer_.advance();
            advanced = true;
            break;

        case TokenKind::End:
        case TokenKind::Error:
            return nullptr;

        default:
            advanced = false;
            break;
        }
    } while (advanced);

    return parseExpr();
}

AstExpr *Parser::parseExpr(BindingPower bindingPower) {
    Token token = lexer_.current();
    AstExpr *expr = grammar_[token.kind].nud(*this, token);
    if (!expr) {
        return nullptr;
    }

    while (bindingPower < grammar_[lexer_.current().kind].bindingPower) {
        token = lexer_.current();
        expr = grammar_[token.kind].led(*this, expr, token);
        if (!expr) {
            return nullptr;
        }
    }

    return expr;
}
