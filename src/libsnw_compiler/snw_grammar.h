#ifndef SNW_GRAMMAR_H
#define SNW_GRAMMAR_H

namespace Snowda {

    class Parser;

    using RootResult = Result<const Ast::Root *, ParserError>;
    using ExprResult = Result<const Ast::Expr *, ParserError>;
    using StmtResult = Result<const Ast::Stmt *, ParserError>;

    class Grammar {
    public:
        using Std = StmtResult(*)(Parser &, Token);
        using Nud = ExprResult(*)(Parser &, Token);
        using Led = ExprResult(*)(Parser &, const Ast::Expr *, Token);

        Grammar();

        int bp(Token token) const;
        ParserResult std(Parser &parser, Token token) const;
        ParserResult nud(Parser &parser, Token token) const;
        ParserResult led(Parser &parser, Ast::NodePtr node, Token token) const;

    private:
        struct Rule {
            int bp;
            Std std;
            Nud nud;
            Led led;

            Rule();
        };

        Rule &getRule(TokenType type);
        const Rule &getRule(TokenType type) const;

        void stmt(TokenType type, Std std);
        void prefix(TokenType type, Nud nud);
        void infix(TokenType type, int bp, Led led);

    private:
        Rule rules_[static_cast<size_t>(TokenType::Count)];
    };

}

#endif // SNW_GRAMMAR_H
