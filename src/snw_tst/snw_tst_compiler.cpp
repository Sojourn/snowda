#include "snw_tst.h"

using namespace Snowda;

void Snowda::tstCompiler()
{
    TestModule module("Compiler");
    tstLexer();
    tstParser();
}

void Snowda::tstLexer()
{
    TestCase test("Lexer");
}

void Snowda::tstParser()
{
    TestCase test("Parser");
}
