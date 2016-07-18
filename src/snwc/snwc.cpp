#include "snwc.h"

const char *src = 
"a = 3"
"b = 4"
"c = a + b + a";

int main(int argc, char **argv) {
    AstTranslator translator(src);
    const Code &code = translator.code();

    std::vector<uint16_t> frame;
    frame.resize(code.stack);
    for (const Instruction &inst: code.insts) {
        switch (inst.kind) {
        case InstructionKind::Load:
            memcpy(&frame[inst.data[0]], &inst.data[1], sizeof(uint16_t));
            break;
        case InstructionKind::Move:
            frame[inst.data[0]] = frame[inst.data[1]];
            break;
        case InstructionKind::Add:
            frame[inst.data[0]] = frame[inst.data[1]] + frame[inst.data[2]];
            break;
        case InstructionKind::Subtract:
            frame[inst.data[0]] = frame[inst.data[1]] - frame[inst.data[2]];
            break;
        default:
            abort();
        }
    }

    for (std::pair<std::string, uint8_t> var : code.vars) {
        std::cout << var.first << " = " << frame[var.second] << std::endl;
    }

    std::system("pause");
    return EXIT_SUCCESS;
}
