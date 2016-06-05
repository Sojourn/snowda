#include "snw_tst.h"

using namespace Snowda;

int main(int argc, char **argv) {
    MemoryManager memoryManager;
    ArenaAllocator &arena = memoryManager.arenaAllocator();
    uint32_t *value = new(arena.allocate(sizeof(uint32_t))) uint32_t;
    *value = 4;
    std::cout << *value << std::endl;

	std::system("pause");
    return 0;
}