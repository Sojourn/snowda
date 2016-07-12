#include "snw.h"

using namespace Snowda;
using namespace Snowda::Ast;

int main(int argc, char **argv)
{
    static const char *src =
        "a = 3\n"
        "b = 4\n"
        "c = a + b\n";

    MemoryManager manager;
    Lexer lexer(src);
    Parser parser(manager, lexer);

    StmtResult result = parser.parseStatement();
    if (result.hasError()) {
        std::cout << result.error() << std::endl;
        return 1;
    }

    //FunctionTranslator xlator(static_cast<const Ast::FnDeclStmt &>(*result.value()));

    // Program program(manager);
    // VirtualMachine virtualMachine(program);
    // virtualMachine.run(View<const char *>(argv, argv + argc));

#ifdef SNW_OS_WIN32
    std::system("pause");
#endif
    return 0;
}
