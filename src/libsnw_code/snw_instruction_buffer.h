#ifndef SNW_INSTRUCTION_BUFFER_H
#define SNW_INSTRUCTION_BUFFER_H

namespace Snowda {

    class InstructionBuffer {
    public:
        InstructionBuffer();

        void freeze();

        Result<InstructionIndex, StringView> emit(InstructionKind kind);
        Result<InstructionIndex, StringView> emit(InstructionKind kind, uint8_t dst, uint16_t src);
        Result<InstructionIndex, StringView> emit(InstructionKind kind, uint8_t dst, uint8_t src);
        Result<InstructionIndex, StringView> emit(InstructionKind kind, uint8_t dst, uint8_t lhs, uint8_t rhs);

        Instruction operator[](InstructionIndex index) const;

    private:
        Result<InstructionIndex, StringView> finishEmit(InstructionIndex index);

    private:
        std::vector<uint8_t> buffer_;
        bool frozen_;
    };
}

#endif // SNW_INSTRUCTION_BUFFER_H
