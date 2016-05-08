#include "snw_compiler.h"

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

size_t Lexer::pos() const
{
    return state_.pos();
}

size_t Lexer::col() const
{
    return state_.col();
}

size_t Lexer::row() const
{
    return state_.row();
}

Token Lexer::next()
{
    consumeWhitespace();

    if (state_.done()) {
        Token token;
        token.type = TokenType::Finished;
        token.content = StringView();
        token.col = col();
        token.row = row();
        return token;
    }

    using Stage = bool(*)(LexerState &, Token &);
    Stage stages[] = {
        &Lexer::syntaxStage,
        &Lexer::identifierStage,
        &Lexer::stringStage,
    };

    for (auto stage: stages) {
        LexerState tmpState = state_;
        Token token;
        if (stage(tmpState, token)) {
            state_ = tmpState;
            return token;
        }
    }

    Token token;
    if (state_.done()) {
        token.type = TokenType::Finished;
        token.content = StringView();
        token.col = col();
        token.row = row();
    }
    else {
        token.type = TokenType::Error;
        token.content = "Lexer unable to generate any tokens";
        token.col = col();
        token.row = row();
    }

    return token;
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
    SNW_TOKENS_KEYWORDS
    SNW_TOKENS_PUNCTUATION
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
            token.content = typeName;
            return true;
        }
    }

    return false;
}

bool Lexer::identifierStage(LexerState &state, Token &token)
{
    assert(!state.done());

    token.type = TokenType::Identifier;
    token.row = state.row();
    token.col = state.col();
    auto begin = state.begin();
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

        auto end = state.begin();
        token.content = StringView(begin, end);
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
        auto begin = state.begin();
        while (!state.done()) {
            auto c = state.peek();
            if (c == '"') {
                auto end = state.begin();
                token.content = StringView(begin, end);
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
