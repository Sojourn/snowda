#ifndef SNW_TST_H
#define SNW_TST_H

#include <iostream>
#include <vector>
#include <memory>

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

#include "snw_util.h"
#include "snw_memory.h"
#include "snw_compiler.h"

#include "snw_tst_util.h"
#include "snw_tst_memory.h"
#include "snw_tst_compiler.h"

namespace Snowda {
    class TestModule {
    public:
        TestModule(const char *name);
        ~TestModule();
    };

    class TestCase {
    public:
        TestCase(const char *name);
        ~TestCase();
    };
}

#endif // SNW_TST_H
