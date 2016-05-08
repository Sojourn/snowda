#ifndef SNW_PARSER_UTIL_H
#define SNW_PARSER_UTIL_H

namespace Snowda {

    namespace BindingPower {
        enum {
            None       = 0,
            Assignment = 10,
            Logical    = 20,
            Relational = 30,
            Sum        = 40,
            Product    = 50,
            Unary      = 60,
            Call       = 70,
        };
    }

    template<TokenType type, int bp, Ast::UnaryOperator op>
    void prefix(Parser &parser) {
        parser.add(type, [](Parser &parser, Token token) -> ParserResult {
            ParserResult result = parser.parseExpression(bp);
            if (result.hasError()) {
                return result;
            }
            else {
                return std::make_unique<UnaryExpression>(op, std::move(result.value()));
            }
        });
    }

    template<TokenType type, int bp, Ast::BinaryOperator op>
    void infix(Parser &parser) {
        parser.add(type, bp, [](Parser &parser, ExpressionPtr left, Token token) -> ParserResult {
            ParserResult result = parser.parseExpression(bp);
            if (result.hasError()) {
                return result;
            }
            else {
                return std::make_unique<BinaryExpression>(op, std::move(left), std::move(result.value()));
            }
        });
    }

}

#endif // SNW_PARSER_UTIL_H
