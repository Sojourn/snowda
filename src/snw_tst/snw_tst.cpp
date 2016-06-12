#include "snw_tst.h"

using namespace Snowda;

TestModule::TestModule(const char *name)
{
    assert(name);
    printf("%s\n", name);
}

TestModule::~TestModule()
{
    printf("\n");
}

TestCase::TestCase(const char *name)
{
    assert(name);
    printf("  %s: ", name);
}

TestCase::~TestCase()
{
    printf(" passed\n");
}

int main(int argc, char **argv) {
    tstUtil();
    tstMemory();
    tstCompiler();

#ifdef SNW_OS_WIN32
	std::system("pause");
#endif
    return 0;
}