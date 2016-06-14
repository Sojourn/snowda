#ifndef SNW_INSTRUCTION_WRITER_H
#define SNW_INSTRUCTION_WRITER_H

namespace Snowda {

    class InstructionWriter {
    public:
        void write(OpCode opCode, uint8_t res, uint8_t lhs, uint8_t rhs);
        void write(OpCode opCode, uint8_t res, uint16_t val);

    private:
        std::vector<Instruction> instructions_;
    };
}

#endif // SNW_INSTRUCTION_WRITER_H
