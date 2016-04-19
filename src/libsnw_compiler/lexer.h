#ifndef SNOWDA_LEXER_H
#define SNOWDA_LEXER_H

#define SNOWDA_TOKENS_KEYWORDS \
    X(Import, "import") \
    X(Def,    "def") \
    X(Var,    "var") \

#define SNOWDA_TOKENS_PUNCTUATION \
    X(Equal   , "==") \
    X(NEqual  , "!=") \
    X(GEqual  , ">=") \
    X(LEqual  , "<=") \
    X(LShift  , "<<") \
    X(RShift  , ">>") \
    X(Assign  , "=") \
    X(LABrace , "<") \
    X(RABrace , ">") \
    X(LParen  , "(") \
    X(RParen  , ")") \
    X(LBrace  , "[") \
    X(RBrace  , "]") \
    X(LCBrace , "{") \
    X(RCBrace , "}") \
    X(And     , "&") \
    X(Or      , "|") \
    X(Bang    , "!") \
    X(XOr     , "^") \
    X(Mult    , "*") \
    X(Div     , "/") \
    X(Mod     , "%") \
    X(Plus    , "+") \
    X(Minus   , "-") \
    X(Pound   , "#") \
    X(Semi    , ";") \
    X(Colon   , ":") \
    X(Dot     , ".") \
    X(Comma   , ",")

#define SNOWDA_TOKENS_OTHER \
    X(String, "string") \
    X(Symbol, "symbol") \
    X(Error,  "error")

#define SNOWDA_TOKENS \
    SNOWDA_TOKENS_KEYWORDS \
    SNOWDA_TOKENS_PUNCTUATION \
    SNOWDA_TOKENS_OTHER

namespace Snowda {

    enum class TokenType {
#define X(xType, xName) xType,
        SNOWDA_TOKENS
#undef X
    };
    StringView name(TokenType type);

    struct Token {
        TokenType  type;
        StringView content;
        size_t     row;
        size_t     col;

        Token();
    };
    std::ostream &operator<<(std::ostream &os, const Token &token);

    class Lexer {
    public:
        explicit Lexer(StringView src);

        bool next(Token &token);

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
        static bool symbolStage(LexerState &state, Token &token);
        static bool stringStage(LexerState &state, Token &token);

    private:
        LexerState state_;
    };
}

#endif // SNOWDA_LEXER_H
