#include "snw.h"

using namespace Snowda;
using namespace Snowda::Ast;

class Type {
};

class Method {
    Method(uint32_t x, View<Instruction>, View<Constants> constants);
};

class Frame {
public:
    Frame(uint32_t type, uint32_t method, uint16_t self);

    uint32_t type() const;
    uint32_t method() const;
    uint16_t self() const;

private:
    union {
        struct {
            uint32_t type;
            uint32_t method;
            uint16_t self;
            uint16_t instructionPointer;
        };

        uint16_t slots[1];
    };
};

class Heap {
public:
    Heap(MemoryManager &manager);

    Frame *pushFrame(uint32_t type, uint32_t method, uint32_t self);
    void popFrame();
};

int run(Instruction *instructions, size_t count)
{
    uint16_t frame[256];
    size_t cursor = 0;
    while (cursor < count) {
        Instruction instruction = instructions[cursor++];
        switch (instruction.opCode) {
        case OpCode::Add:
            frame[instruction.a.res] = frame[instruction.a.lhs] + frame[instruction.a.rhs];
            break;
        case OpCode::Sub:
            frame[instruction.a.res] = frame[instruction.a.lhs] - frame[instruction.a.rhs];
            break;
        case OpCode::Move:
            frame[instruction.a.res] = frame[instruction.a.lhs];
            break;
        case OpCode::Load:
            frame[instruction.b.res] = instruction.b.val;
            break;
        case OpCode::Print:
            std::cout << frame[instruction.a.res] << std::endl;
            break;
        case OpCode::Halt:
            return 0;
        default:
            return -1;
        }
    }

	return -1;
}

int main(int argc, char **argv)
{
    Instruction instructions[] = {
       { OpCode::Load, { { 0, 15 } } },
       { OpCode::Load, { { 1, 7 } } },
       { OpCode::Add, { { 2, 0, 1 } } },
       { OpCode::Print, { { 0, 0, 0 } } },
       { OpCode::Print, { { 1, 0, 0 } } },
       { OpCode::Print, { { 2, 0, 0 } } },
       { OpCode::Halt, { { 0, 0, 0 } } },
    };

    run(instructions, 6);

    // std::vector<Instruction> instructions = writer.instructions();
    // run(instructions.data(), instructions.size());

#ifdef SNW_OS_WIN32
    std::system("pause");
#endif
    return 0;
}
