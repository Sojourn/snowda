#ifndef SNW_ALLOCATOR_PAGE_H
#define SNW_ALLOCATOR_PAGE_H

namespace Snowda {

    struct AllocationHeader {
        uint8_t size;
        uint8_t flags;
        uint16_t reserved;
        uint32_t type;
    };

    struct AllocatorPageHeader {
        uint8_t index;
        uint8_t live;
        uint8_t dead;
        uint8_t next;
        uint64_t reserved;
    };
    static_assert(sizeof(AllocatorPageHeader) == 16, "Bad allocator page header layout");

    struct AllocatorPage {
        PageHeader header;
        uint8_t data[4096 - sizeof(AllocatorPageHeader)];
    };
    static_assert(sizeof(AllocatorPage) == 4096, "Bad allocator page layout");

}

#endif // SNW_ALLOCATOR_PAGE_H
