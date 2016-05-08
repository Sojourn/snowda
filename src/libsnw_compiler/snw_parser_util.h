#ifndef SNW_PARSER_UTIL_H
#define SNW_PARSER_UTIL_H

namespace Snowda {

    template<TokenType type>
    struct LookupUnaryOperator;

    template<TokenType type>
    struct LookupBinaryOperator;

    template<TokenType type, int bp>
    NullDelimitedFunc prefix() {
        return [](Parser &parser) -> ParserResult {
            return ParserError(parser, "Not implemented");
        };
    }

    template<TokenType type, int bp>
    LeftDelimitedFunc infix() {
        return [](Parser &parser) -> ParserResult {
            return ParserError(parser, "Not implemented");
        };
    }

}

#endif // SNW_PARSER_UTIL_H
