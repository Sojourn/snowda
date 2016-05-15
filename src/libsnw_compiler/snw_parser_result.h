#ifndef SNW_PARSER_RESULT_H
#define SNW_PARSER_RESULT_H

namespace Snowda {

    class Parser;

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

    using ParserResult = Result<Ast::NodePtr, ParserError>;
}

#endif // SNW_PARSER_RESULT_H
