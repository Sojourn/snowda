#ifndef SNWC_GRAMMAR_H
#define SNWC_GRAMMAR_H

class Parser;

enum class BindingPower {
    None       = 0,
    Assignment = 10,
    Logical    = 20,
    Relational = 30,
    Sum        = 40,
    Product    = 50,
    Unary      = 60,
    Call       = 70,
};

struct GrammarRule {
    BindingPower bindingPower;
    AstExpr *(*nud)(Parser &parser, Token token);
    AstExpr *(*led)(Parser &parser, AstExpr *left, Token token);
};

class Grammar {
public:
    Grammar();

    GrammarRule &operator[](TokenKind kind);
    const GrammarRule &operator[](TokenKind kind) const;

private:
    static AstExpr *defaultNud(Parser &parser, Token token);
    static AstExpr *defaultLed(Parser &parser, AstExpr *left, Token token);

    template<UnaryOperationKind operatorKind>
    static AstExpr *unaryOperationNud(Parser &parser, Token token);

    template<BinaryOperationKind operatorKind>
    static AstExpr *binaryOperationLed(Parser &parser, AstExpr *left, Token token);

    static AstExpr *assignLed(Parser &parser, AstExpr *left, Token token);

    static AstExpr *numberNud(Parser &parser, Token token);
    static AstExpr *symbolNud(Parser &parser, Token token);

private:
    std::array<GrammarRule, static_cast<size_t>(TokenKind::Count)> rules_;
};

#endif // SNWC_GRAMMAR_H
