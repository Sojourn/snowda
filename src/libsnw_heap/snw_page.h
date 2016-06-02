#ifndef SNW_PAGE_H
#define SNW_PAGE_H

namespace Snowda {

    using PhysicalAddress = uint8_t *;
    using VirtualAddress = uint16_t;

    union Page {
        FreePage freePage;
        BlockPage blockPage;
        BufferPage bufferPage;
        AllocatorPage allocatorPage;
    };

}

#endif // SNW_PAGE_H
