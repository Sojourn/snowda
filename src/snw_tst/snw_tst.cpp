#include "snw_tst.h"

using namespace Snowda;

class Number {
    using NumberPtr = nullptr_t;

    static NumberPtr parse(StringView str)
    {
        // All times are positive - the minus operator can be used to
        // create a negative number

        if (str[0] == '0') {
            if (str[1] == 'x') {
                // hex
            }
            else {
                // octal
            }
        }
        else if (str.endsWith("u8")) {
        }
        // ...
        else {
            // Parse as highest resolution positive number (uint64_t)
        }

        return nullptr;
    }
};

class Integer : public Number {};
class IntegerU8 : public Number {};
class IntegerI8 : public Number {};
// ...

class Float : public Number {};
class Float32 : public Float {};
class Float64 : public Float {};

// Duration of time since the epoch
class Instant : public Number {
    // timespec_t ts;
};

class Time : public Number {
    // Date
    int year;
    int month;
    int day;

    // Time
    int hour;
    int minute;
    int second;
    int millisecond;
    int microsecond;
    int nanosecond;
};

class Interval : public Number {
    // ... 
};

int main(int argc, char **argv) {
	assert(StringView("asdf").startsWith("as"));
	assert(!StringView("asdf").endsWith("as"));
	assert(StringView("asdf").endsWith("df"));

    return 0;
}