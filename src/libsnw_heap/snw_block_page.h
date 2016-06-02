#ifndef SNW_BLOCK_H
#define SNW_BLOCK_H

namespace Snowda {

    enum class PageType : uint8_t {
        FreePage,
        BlockPage,
        BufferPage,
        AllocatorPage,
    };

    enum class PageOwnerType : uint8_t {
        Process,
        Driver,
    };

    using PageOwner = uint32_t;

    struct BlockPageEntry {
        PageOwner owner;
        PageOwnerType ownerType;
        PageType pageType;
        uint16_t reserved;
    };
    static_assert(sizeof(BlockPageEntry) == 8, "Bad block page entry layout");

    struct BlockPage {
        BlockPageEntry entries[512];
    };
    static_assert(sizeof(BlockPage) == 4096, "Bad block page layout");

    struct Block {
        BlockPage blockPage;
        std::array<Page, ((2 << 20) - sizeof(BlockPage)) / sizeof(Page)> pages;
    };
    static_assert(sizeof(Block) == (2 << 20), "Bad block layout");
}

#endif // SNW_BLOCK_H
