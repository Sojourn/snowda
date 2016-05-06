#ifndef SNW_TOKEN_STREAM_H
#define SNW_TOKEN_STREAM_H

namespace Snowda {

    class TokenStream {
    public:
        explicit TokenStream(Lexer lexer);

        size_t pos() const;
        size_t row() const;
        size_t col() const;

        Token operator[](size_t index);
        void advance(size_t count);

    private:
        enum {
            size = 8,
            mask = size - 1,
        };

        std::array<Token, size> buffer_;
        Lexer  lexer_;
        size_t head_;
        size_t tail_;
    };
}

#endif // SNW_TOKEN_STREAM_H
