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
