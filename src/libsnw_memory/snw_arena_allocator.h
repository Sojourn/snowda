#ifndef SNW_ARENA_ALLOCATOR_H
#define SNW_ARENA_ALLOCATOR_H

namespace Snowda {

    class ArenaAllocator {
    public:
        ArenaAllocator(MemoryManager &manager);
        ArenaAllocator(const ArenaAllocator &) = delete;
        ArenaAllocator(ArenaAllocator &&) = delete;
        ~ArenaAllocator();

        ArenaAllocator &operator=(const ArenaAllocator &) = delete;
        ArenaAllocator &operator=(ArenaAllocator &&) = delete;

        size_t capacity() const;

        void addBuffer(Buffer buffer);
        Buffer removeBuffer();

        uint8_t *allocate(size_t size);
        void clear();

    private:
        MemoryManager &manager_;
        std::vector<Buffer> buffers_;
        std::vector<uint8_t *> slop_;
        size_t capacity_;
        size_t bufferIndex_;
        size_t bufferTop_;
    };

}

#endif // SNW_ARENA_ALLOCATOR_H
