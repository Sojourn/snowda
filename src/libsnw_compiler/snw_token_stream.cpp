#include "snw_compiler.h"

using namespace Snowda;

TokenStream::TokenStream(Lexer lexer)
    : lexer_(lexer)
    , head_(0)
    , tail_(0)
{
}

size_t TokenStream::pos() const
{
    return lexer_.pos();
}

size_t TokenStream::col() const
{
    return lexer_.col();
}

size_t TokenStream::row() const
{
    return lexer_.row();
}

Token TokenStream::operator[](size_t index)
{
    assert(index < size);

    while ((head_ - tail_) <= index) {
        Token token = lexer_.next();
        buffer_[head_ & mask] = token;
        head_ += 1;
    }

    return buffer_[(tail_ + index) & mask];
}

void TokenStream::advance(size_t count)
{
    assert(count <= (head_ - tail_));

    tail_ += count;
}
