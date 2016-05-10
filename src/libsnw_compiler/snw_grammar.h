#ifndef SNW_GRAMMAR_H
#define SNW_GRAMMAR_H

namespace Snowda {

    class Parser;

    class Grammar {
    public:
        using Expr = Ast::ExpressionPtr;
        using Nud = ParserResult(*)(Parser &, Token);
        using Led = ParserResult(*)(Parser &, Expr, Token);

        Grammar();

        int bp(Token token) const;
        ParserResult nud(Parser &parser, Token token) const;
        ParserResult led(Parser &parser, Expr expr, Token token) const;

    private:
        struct Rule {
            int bp;
            Nud nud;
            Led led;

            Rule();
        };

        Rule &getRule(TokenType type);
        const Rule &getRule(TokenType type) const;

        void addRule(TokenType type, Nud func);
        void addRule(TokenType type, int bp, Led func);

    private:
        Rule rules_[static_cast<size_t>(TokenType::Count)];
    };

}

#endif // SNW_GRAMMAR_H
