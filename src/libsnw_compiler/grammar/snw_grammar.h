#ifndef SNW_GRAMMAR_H
#define SNW_GRAMMAR_H

namespace Snowda {
    
    using ExprResult = Result<const Ast::Expr *, ParserError>;
    using StmtResult = Result<const Ast::Stmt *, ParserError>;
    using RootResult = Result<const Ast::RootStmt *, ParserError>;

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

    class Grammar {
    public:
        using Std = StmtResult(*)(Parser &, Token);
        using Nud = ExprResult(*)(Parser &, Token);
        using Led = ExprResult(*)(Parser &, const Ast::Expr *, Token);

        Grammar();

        int bp(Token token) const;
        StmtResult std(Parser &parser, Token token) const;
        ExprResult nud(Parser &parser, Token token) const;
        ExprResult led(Parser &parser, const Ast::Expr *expr, Token token) const;

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
