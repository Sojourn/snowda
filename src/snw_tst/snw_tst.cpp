#include "snw_tst.h"

using namespace Snowda;

using Page = std::array<uint8_t, 4096>;

struct PageBlockEntry {
    uint32_t owner;
    uint16_t flags;
    uint16_t reserved;
};

// Must be (2 << 20) aligned
struct BlockPage {
    PageAllocator *pageAllocator;
    PageBlockEntry entries[512 - 1];
};
static_assert(sizeof(BlockPage) == sizeof(Page), "Bad block page layout");

// using ServiceMap = std::array<Service *, 256>;

class PageAllocator {
public:
    Page *allocatePage(uint32_t owner, uint8_t ownerType, uint16_t tag);
    void deallocatePage(Page *page);
};

struct ProcessHeader {
    PageSet pageSet;
    Page *pages[256];
    uint64_t registers[32];
    uint32_t registerTypes[32];
    uint16_t stackPosition;
};

struct Process {
    ProcessHeader header;
    std::array<VirtualAddress, (4096 - header) / sizeof(VirtualAddress)> stack;
};
static_assert(sizeof(Process) == 4096, "Bad process layout");

union ProcessId {
    struct {
        uint32_t threadPart : 8;
        uint32_t processPart : 24;
    };

    uint32_t value;
};

class ProcessHeap {
public:
    ProcessHeap(ProcessId id, PageAllocator &allocator);

    BufferPage *allocateBufferPage();
    void freeBufferPage(BufferPage *bufferPage);

    // Return nullptr on failure
    Object *allocateObject(ObjectType type);
    void freeObject(Object *object);

    // Return nullptr on failure
    ObjectArray *allocateObjectArray(ObjectType type, size_t count);
    void freeObjectArray(ObjectArray *objectArray);

    bool attachPage(Page *page);
    void detachPage(Page *page);

    virtual void garbageCollect() = 0;
    virtual void compact() = 0;

private:
    PageAllocator &allocator_;
    std::vector<Page *> pages_;
    PageSet pageSet_;
    const ProcessId id_;
};

int main(int argc, char **argv) {
    return 0;
}