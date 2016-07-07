#ifndef SNWC_AST_TRANSLATOR_H
#define SNWC_AST_TRANSLATOR_H

enum class InstructionKind : uint8_t {
    Load,
    Move,
    Add,
    Subtract,
    Print,
};

struct Instruction {
    InstructionKind kind;
    uint8_t data[3];
};

struct Code {
    std::vector<Instruction>       insts;
    std::map<std::string, uint8_t> vars;
    uint8_t                        stack;
};

class AstTranslator {
public:
    AstTranslator(const char *src);

    const Code &code() const;

public:
    uint8_t visit(UnaryOperationAstExpr *expr);
    uint8_t visit(BinaryOperationAstExpr *expr);
    uint8_t visit(AssignAstExpr *expr);
    uint8_t visit(NumberAstExpr *expr);
    uint8_t visit(SymbolAstExpr *expr);

private:
    uint8_t getTempVariable();
    uint8_t getNamedVariable(std::string name);
    void emit(Instruction inst);

private:
    const char *src_;
    Code        code_;
};

#endif // SNWC_AST_TRANSLATOR_H
