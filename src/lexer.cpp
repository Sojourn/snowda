#include "snowda.h"

using namespace Snowda;

namespace {
    bool alpha(char c)
    {
        return ((c >= 'a') && (c <= 'z')) ||
               ((c >= 'A') && (c <= 'Z'));
    }

    bool digit(char c)
    {
        return (c >= '0') && (c <= '9');
    }

    bool whitespace(char c)
    {
        return (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n');
    }
}

StringView Snowda::name(TokenType type)
{
    switch(type) {
#define X(xType, xName) case TokenType::xType: return StringView(xName);
    SNOWDA_TOKENS
#undef X
        default: abort();
    }
}

std::ostream &Snowda::operator<<(std::ostream &os, const Token &token)
{
    os << "Token {\n";
    os << "  type: " << name(token.type) << '\n';
    os << "  row: " << token.row << '\n';
    os << "  col: " << token.col << '\n';
    switch (token.type) {
    case TokenType::String:
        os << "  value: " << StringView(token.string.begin, token.string.end) << '\n';
        break;
    case TokenType::Symbol:
        os << "  value: " << StringView(token.symbol.begin, token.symbol.end) << '\n';
        break;
    case TokenType::Error:
        os << "  value: " << StringView(token.error.reason) << '\n';
        break;
    default:
        break;
    }
    os << "}";

    return os;
}

Lexer::Lexer(StringView src)
    : state_(src)
{
}

Lexer::LexerState::LexerState(StringView src)
    : src_(src)
    , pos_(0)
    , col_(0)
    , row_(0)
{
}

size_t Lexer::LexerState::pos() const
{
    return pos_;
}

size_t Lexer::LexerState::col() const
{
    return col_;
}

size_t Lexer::LexerState::row() const
{
    return row_;
}

StringView Lexer::LexerState::str() const
{
    return src_.tail(pos_);
}

const char *Lexer::LexerState::begin() const
{
    return src_.begin() + pos_;
}

const char *Lexer::LexerState::end() const
{
    return src_.end();
}

bool Lexer::LexerState::done() const
{
    return pos_ == src_.size();
}

char Lexer::LexerState::peek() const
{
    assert(!done());

    return src_[pos_];
}

char Lexer::LexerState::next()
{
    const char c = peek();
    advance();
    return c;
}

void Lexer::LexerState::advance(size_t count)
{
    assert((pos_ + count) <= src_.size());

    for (size_t i = 0; i < count; ++i) {
        switch (src_[pos_ + i]) {
        case '\t':
            col_ += 4;
            break;
        case '\r':
            col_ = 0;
            break;
        case '\n':
            col_ = 0;
            row_ += 1;
            break;
        default:
            col_ += 1;
        }
    }

    pos_ += count;
}

bool Lexer::next(Token &token)
{
    consumeWhitespace();

    if (state_.done()) {
        return false;
    }

    using Stage = bool(*)(LexerState &, Token &);
    Stage stages[] = {
        &Lexer::syntaxStage,
        &Lexer::symbolStage,
        &Lexer::stringStage,
    };

    for (auto stage: stages) {
        LexerState tmpState = state_;
        Token tmpToken;
        if (stage(tmpState, tmpToken)) {
            state_ = tmpState;
            token = tmpToken;
            return true;
        }
    }

    if (state_.done()) {
        return false;
    }
    else {
        token.type = TokenType::Error;
        token.error.reason = "Lexer unable to generate any tokens";
        return true;
    }
}

void Lexer::consumeWhitespace()
{
    while (!state_.done() && whitespace(state_.peek())) {
        state_.advance();
    }
}

bool Lexer::syntaxStage(LexerState &state, Token &token)
{
    assert(!state.done());

    // Longer tokens match first
    TokenType types[] = {
#define X(xType, xName) TokenType::xType,
    SNOWDA_TOKENS_KEYWORDS
    SNOWDA_TOKENS_PUNCTUATION
#undef X
    };

    token.row = state.row();
    token.col = state.col();
    for (auto type: types) {
        auto str = state.str();
        auto typeName = name(type);
        if (str.startsWith(typeName)) {
            state.advance(typeName.size());
            token.type = type;
            return true;
        }
    }

    return false;
}

bool Lexer::symbolStage(LexerState &state, Token &token)
{
    assert(!state.done());

    token.type = TokenType::Symbol;
    token.row = state.row();
    token.col = state.col();
    token.symbol.begin = state.begin();
    if (alpha(state.next())) {
        while (!state.done()) {
            auto c = state.peek();
            if (alpha(c) || digit(c)) {
                state.advance();
            }
            else {
                break;
            }
        }

        token.symbol.end = state.begin();
        return true;
    }

    return false;
}

bool Lexer::stringStage(LexerState &state, Token &token)
{
    assert(!state.done());

    token.type = TokenType::String;
    token.row = state.row();
    token.col = state.col();
    if (state.next() == '"') {
        token.string.begin = state.begin();
        while (!state.done()) {
            auto c = state.peek();
            if (c == '"') {
                token.string.end = state.begin();
                state.advance();
                return true;
            }
            else {
                state.advance();
            }
        }
    }

    return false;
}
