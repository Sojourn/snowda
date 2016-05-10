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

class Heap;

struct HeapPageHeader {
    Heap    *heap;
    uint16_t index;
    uint16_t live;
    uint16_t top;
    uint16_t reserved;
};
// FIXME: The windows build is 32 bit, so this fails...
// static_assert(sizeof(HeapPageHeader) == 12, "Unexpected HeapPageHeader size");

// Allocate objects with a slab allocator. If memory needs to be made available (to the vm or fiber)
// do a compaction. This should be really fast since the heap is small. Otherwise mark pages as clean
// when the live count drops to zero. Could also have a work-stealing list of fibers to compact.
struct HeapPage {
    HeapPageHeader header;
    uint8_t data[4096 - sizeof(HeapPageHeader)];
};
 
// Manages a tiny per-fiber heap. Fibers store bulk data in a MemoryStream. Want to
// encourage a small working set that can fit in cache and serial access to less
// frequently used data in ram. Need to think about how a collections library should work
// with this in mind. Vector, Map(Hash/BTree), Queue, List. Heap-only/Stream-only/Hybrid.
// This doesn't need to be Ram either, should expose a c-api that could be used for disk/hdfs/memcached.
// Also think about keeping fibers/memory pinned to a core/numa node.
class Heap {
public:
    uint8_t *allocate(size_t size); // Returns nullptr on failure (specialized option instead?)
    void free(uint8_t *buf);

private:
    // 4096 * 128 = 512KB for the stack + heap
    // The entire heap can be addressed with a uint16_t with 8 byte alignment. (8 * (1 << 16)) == 512KB.
    //   Addresses need to be translated obviously, but it should still be a win. Pointers to other vm resources
    //   will still need to use full 8 byte addresses.
    // freePages_ | dirtyPages_ -> the corresponding pages pointer will be set..
    // Can use intrinsics to find pages to free / allocate from with one or two instructions
    uint64_t freePages_[2];
    uint64_t dirtyPages_[2];
    HeapPage *pages_[128]; // It might make sense to dynamically allocate/resize this array
};

int main(int argc, char **argv) {
	assert(StringView("asdf").startsWith("as"));
	assert(!StringView("asdf").endsWith("as"));
	assert(StringView("asdf").endsWith("df"));

    return 0;
}