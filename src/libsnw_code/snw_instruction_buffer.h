#ifndef SNW_INSTRUCTION_BUFFER_H
#define SNW_INSTRUCTION_BUFFER_H

namespace Snowda {
    class InstructionBuffer {
    public:
        InstructionBuffer();

        void freeze();

        Result<uint16_t, StringView> emit(InstructionKind kind);
        Result<uint16_t, StringView> emit(InstructionKind kind, uint8_t dst, uint16_t src);
        Result<uint16_t, StringView> emit(InstructionKind kind, uint8_t dst, uint8_t src);
        Result<uint16_t, StringView> emit(InstructionKind kind, uint8_t dst, uint8_t lhs, uint8_t rhs);

        Instruction operator[](uint16_t index) const;

    private:
        Result<uint16_t, StringView> finishEmit(uint16_t label);

    private:
        std::vector<uint8_t> buffer_;
        bool frozen_;
    };
}

#endif // SNW_INSTRUCTION_BUFFER_H
