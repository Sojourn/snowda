#ifndef SNW_PARSER_ERROR_H
#define SNW_PARSER_ERROR_H

namespace Snowda {

    class ParserError {
    public:
        ParserError(const Token &token);
        ParserError(const Token &token, const StringView &msg);

        const StringView &msg() const;
        size_t col() const;
        size_t row() const;

    private:
        const StringView msg_;
        const size_t col_;
        const size_t row_;
    };

}

#endif // SNW_PARSER_ERROR_H
