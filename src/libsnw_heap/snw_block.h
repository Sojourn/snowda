#ifndef SNW_BLOCK_H
#define SNW_BLOCK_H

namespace Snowda {

    struct BlockEntry {
        uint32_t owner;
        uint32_t flags;
    };

    class Block {
    public:
        void clear();

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
