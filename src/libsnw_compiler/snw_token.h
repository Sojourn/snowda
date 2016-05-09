#ifndef SNW_TOKEN_H
#define SNW_TOKEN_H

#define SNW_TOKENS_KEYWORDS \
    X(Import, "import") \
    X(Def,    "def") \
    X(Var,    "var") \

#define SNW_TOKENS_PUNCTUATION \
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
    X(Tilde   , "~") \
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

#define SNW_TOKENS_OTHER \
    X(Number,     "number") \
    X(Character,  "character") \
    X(String,     "string") \
    X(Identifier, "identifier") \
    X(Finished,   "finished") \
    X(Error,      "error")

#define SNW_TOKENS \
    SNW_TOKENS_KEYWORDS \
    SNW_TOKENS_PUNCTUATION \
    SNW_TOKENS_OTHER

namespace Snowda {

    enum class TokenType {
#define X(xType, xName) xType,
        SNW_TOKENS
#undef X
        Count,
    };

    struct Token {
        TokenType  type;
        StringView content;
        size_t     row;
        size_t     col;

        Token();
    };


    StringView name(TokenType type);
    std::ostream &operator<<(std::ostream &os, const Token &token);

}

#endif // SNW_TOKEN_H
