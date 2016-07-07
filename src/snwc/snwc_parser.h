#ifndef SNWC_PARSER_H
#define SNWC_PARSER_H

class Parser {
public:
    Parser(const char *src);

    Lexer &lexer();
    const Lexer &lexer() const;
    
    Grammar &grammar();
    const Grammar &grammar() const;

    AstExpr *parseStmt();
    AstExpr *parseExpr(BindingPower bindingPower = BindingPower::None);

private:
    Lexer lexer_;
    Grammar grammar_;
};

#endif // SNWC_PARSER_H
