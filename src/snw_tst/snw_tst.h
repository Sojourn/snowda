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
// #include "snw_type.h"
#include "snw_memory.h"
#include "snw_compiler.h"

#include "snw_tst_util.h"
#include "snw_tst_memory.h"
#include "snw_tst_compiler.h"

namespace Snowda {
    class String {
        struct StringBuffer {
            size_t size;
            uint8_t data[1];
        };

    public:
        String()
            : value_(0)
        {
        }

        String(StringView str)
        {
            if (str.empty()) {
                value_ = 0;
            }
            else if (str.size() < 7) {
                memcpy(bytes_, str.begin(), str.size());
                bytes_[str.size()] = '\0';
                bytes_[sizeof(bytes_) - 1] = static_cast<uint8_t>(str.size());
            }
            else {
                uint8_t *buffer = new uint8_t[sizeof(size_t) + str.size() + 1];
                StringBuffer *stringBuffer = reinterpret_cast<StringBuffer *>(buffer);
                stringBuffer->size = str.size();

                memcpy(stringBuffer->data, str.begin(), stringBuffer->size);
                stringBuffer->data[stringBuffer->size] = '\0';

                value_ = reinterpret_cast<uintptr_t>(buffer);
            }
        }

        ~String()
        {
            if (isLarge()) {
                delete [] reinterpret_cast<uint8_t *>(value_);
                value_ = 0;
            }
        }

        size_t size() const
        {
            if (isSmall()) {
                return bytes_[sizeof(bytes_) - 1];
            }
            else {
                return stringBuffer().size;
            }
        }

        const char *begin() const
        {
            if (isSmall()) {
                return reinterpret_cast<const char *>(bytes_);
            }
            else {
                return reinterpret_cast<const char *>(stringBuffer().data);
            }
        }

        const char *end() const
        {
            return begin() + size();
        }

    private:
        bool isSmall() const
        {
            return (value_ == 0) || ((bytes_[sizeof(bytes_) - 1] & (sizeof(uint8_t) - 1)) > 0);
        }

        bool isLarge() const
        {
            return !isSmall();
        }

        StringBuffer &stringBuffer()
        {
            assert(isLarge());

            return *reinterpret_cast<StringBuffer *>(value_);
        }

        const StringBuffer &stringBuffer() const
        {
            assert(isLarge());

            return *reinterpret_cast<const StringBuffer *>(value_);
        }

    private:
        union {
            uint8_t bytes_[sizeof(uintptr_t)];
            uintptr_t value_;
        };
    };

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
