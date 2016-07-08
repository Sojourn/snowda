#ifndef SNW_FUNCTION_TRANSLATOR_H
#define SNW_FUNCTION_TRANSLATOR_H

namespace Snowda {

    struct Register {
        uint8_t id;
        uint16_t typeId;
        uint16_t birth;
        uint16_t death;
    };

    class Function {
    public:
        std::vector<Register> registers;
        std::vector<uint8_t>  instructions;
    };

    class FunctionTranslator {
    public:
        FunctionTranslator();

        Result<Function, StringView> result();

    public:
        uint8_t visit(const Ast::NumberExpr &expr);
        uint8_t visit(const Ast::CharacterExpr &expr);
        uint8_t visit(const Ast::StringExpr &expr);
        uint8_t visit(const Ast::IdentifierExpr &expr);
        uint8_t visit(const Ast::BinaryExpr &expr);
        uint8_t visit(const Ast::UnaryExpr &expr);
        uint8_t visit(const Ast::CallExpr &expr);
        uint8_t visit(const Ast::DerefExpr &expr);
        uint8_t visit(const Ast::AssignExpr &expr);
        uint8_t visit(const Ast::ExprStmt &stmt);
        uint8_t visit(const Ast::Node &node);

    private:
        void emit(InstructionKind kind);
        void emit(InstructionKind kind, uint8_t dst, uint16_t src);
        void emit(InstructionKind kind, uint8_t dst, uint8_t src);
        void emit(InstructionKind kind, uint8_t dst, uint8_t lhs, uint8_t rhs);

    private:
        Function function_;
        StringView error_;
        uint8_t nextRegisterId_;
    };

}

#endif // SNW_FUNCTION_TRANSLATOR_H