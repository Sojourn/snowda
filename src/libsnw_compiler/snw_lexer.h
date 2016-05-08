#ifndef SNW_LEXER_H
#define SNW_LEXER_H

namespace Snowda {

    class Lexer {
    public:
        explicit Lexer(StringView src);

        Token next();

        size_t pos() const;
        size_t col() const;
        size_t row() const;

    private:
        class LexerState {
        public:
            explicit LexerState(StringView src);

            size_t pos() const;
            size_t col() const;
            size_t row() const;

            StringView str() const;
            const char *begin() const;
            const char *end() const;

            bool done() const;
            char peek() const;
            char next();
            void advance(size_t count = 1);

        private:
            StringView src_;
            size_t     pos_;
            size_t     col_;
            size_t     row_;
        };

        void consumeWhitespace();
        static bool syntaxStage(LexerState &state, Token &token);
        static bool identifierStage(LexerState &state, Token &token);
        static bool stringStage(LexerState &state, Token &token);

    private:
        LexerState state_;
    };
    
}

#endif // SNW_LEXER_H
