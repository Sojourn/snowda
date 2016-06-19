#include "snw.h"

using namespace Snowda;

int main(int argc, char **argv)
{
    MemoryManager manager;
    Program program(manager);

    VirtualMachine virtualMachine(program);
    virtualMachine.run(View<const char *>(argv, argv + argc));

#ifdef SNW_OS_WIN32
    std::system("pause");
#endif
    return 0;
}
