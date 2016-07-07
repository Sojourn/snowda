#ifndef SNWC_LEXER_H
#define SNWC_LEXER_H

enum class TokenKind {
    Plus,
    Minus,
    Assign,
    Number,
    Symbol,
    Newline,

    Begin,
    End,
    Error,
    Count,
};

struct Token {
    TokenKind kind;
    size_t    pos;
    size_t    len;
};

class Lexer {
public:
    Lexer(const char *src);

    void advance();
    bool advance(TokenKind kind);
    Token current() const;
    Token next() const;

private:
    static bool isAlpha(char c);
    static bool isDigit(char c);
    static bool isWhitespace(char c);

    Token lex();

private:
    const char *src_;
    size_t      pos_;
    Token       tokens_[2];
};

#endif // SNWC_LEXER_H
