#ifndef SNW_BLOCK_H
#define SNW_BLOCK_H

namespace Snowda {

    enum class PageUse : uint8_t {
        None,
        Heap,
        Arena,
    };

    class BlockEntry {
    public:
        BlockEntry();

        uint32_t owner() const;
        void setOwner(uint32_t owner);

        PageUse use() const;
        void setUse(PageUse use);

    private:
        uint32_t owner_;
        PageUse use_;
        uint8_t pad1_;
        uint16_t pad2_;
    };
    static_assert(sizeof(BlockEntry) == sizeof(uint64_t), "Bad block entry layout");

    class Block {
    public:
        Block();

        BlockEntry &entry(size_t index);
        const BlockEntry &entry(size_t index) const;
        BlockEntry &entry(Page &page);
        const BlockEntry &entry(const Page &page) const;

        Page &page(size_t index);
        const Page &page(size_t index) const;

        size_t pageCount() const;

    private:
        size_t index(const Page &page) const;

    private:
        uint64_t reserved_;
        std::array<BlockEntry, 512 - 1> entries_;
        Page pages_[512 - 1];
    };

}

#endif // SNW_BLOCK_H
