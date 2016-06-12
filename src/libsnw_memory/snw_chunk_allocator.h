#ifndef SNW_CHUNK_ALLOCATOR_H
#define SNW_CHUNK_ALLOCATOR_H

namespace Snowda {

    // TODO: Buddy allocator?
    class ChunkAllocator {
    public:
        ChunkAllocator(MemoryManager &manager);
        ~ChunkAllocator();

        uint8_t *allocate(size_t size);
        void deallocate(uint8_t *addr, size_t size);

    private:
        using ChunkList = std::vector<uint8_t *>;

        enum {
            minChunkShift = 4,
            maxChunkShift = 12,
            chunkShiftCount = (maxChunkShift - minChunkShift) + 1,
            minChunkSize = (1 << minChunkShift),
            maxChunkSize = (1 << maxChunkShift),
        };

		size_t findChunkList(size_t size) const;

        MemoryManager &manager_;
        std::vector<Page *> pages_;
        std::array<ChunkList, chunkShiftCount> chunkLists_;
    };

}

#endif // SNW_CHUNK_ALLOCATOR_H
