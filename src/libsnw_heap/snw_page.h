#ifndef SNW_PAGE_H
#define SNW_PAGE_H

namespace Snowda {

    using PhysicalAddress = uint8_t *;
    using VirtualAddress = uint16_t;

    struct ObjectHeader {
        uint8_t size;
        uint8_t flags;
        uint16_t reserved;
        uint32_t type;
    };

    struct PageHeader {
        uint8_t index;
        uint8_t live;
        uint8_t dead;
        uint8_t next;
        uint64_t reserved;
    };
    static_assert(sizeof(PageHeader) == 16, "Bad page header layout");

    struct Page {
        PageHeader header;
        uint8_t data[4096 - sizeof(PageHeader)];
    };
    static_assert(sizeof(Page) == 4096, "Bad page layout");

}

#endif // SNW_PAGE_H
