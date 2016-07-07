#include "snwc.h"

Lexer::Lexer(const char *src)
    : src_(src)
    , pos_(0)
{
    assert(src_ != nullptr);

    tokens_[0].kind = TokenKind::Begin;
    tokens_[0].pos = 0;
    tokens_[0].len = 0;

    tokens_[1] = lex();
}

void Lexer::advance() {
    tokens_[0] = tokens_[1];
    tokens_[1] = lex();
}

bool Lexer::advance(TokenKind kind) {
    if (tokens_[0].kind == kind) {
        advance();
        return true;
    }
    else {
        return false;
    }
}

Token Lexer::current() const {
    return tokens_[0];
}

Token Lexer::next() const {
    return tokens_[1];
}

bool Lexer::isAlpha(char c) {
    return ((c >= 'a') && (c <= 'z')) ||
           ((c >= 'A') && (c <= 'Z'));
}

bool Lexer::isDigit(char c) {
    return (c >= '0') && (c <= '9');
}

bool Lexer::isWhitespace(char c) {
    return (c == ' ') || (c == '\t');
}

Token Lexer::lex() {
    char c = src_[pos_];
    while (isWhitespace(c)) {
        pos_ += 1;
        c = src_[pos_];
    }

    Token token;
    token.pos = pos_;
    if (isDigit(c)) {
        token.kind = TokenKind::Number;
        pos_ += 1;
        do {
            c = src_[pos_];
        } while(isDigit(c));
    }
    else if (isAlpha(c)) {
        token.kind = TokenKind::Symbol;
        pos_ += 1;
        do {
            c = src_[pos_];
        } while(isAlpha(c) || isDigit(c) || (c == '_'));
    }
    else if (c == '=') {
        token.kind = TokenKind::Assign;
    }
    else if (c == '+') {
        token.kind = TokenKind::Plus;
    }
    else if (c == '-') {
        token.kind = TokenKind::Minus;
    }
    else if (c == '\n') {
        token.kind = TokenKind::Newline;
    }
    else if (c == '\0') {
        token.kind = TokenKind::End;
    }
    else {
        token.kind = TokenKind::Error;
    }

    token.len = pos_ - token.pos;
    return token;
}
