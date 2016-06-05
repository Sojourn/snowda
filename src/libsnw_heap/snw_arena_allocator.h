#ifndef SNW_ARENA_ALLOCATOR_H
#define SNW_ARENA_ALLOCATOR_H

namespace Snowda {

    class ArenaAllocator {
    public:
        ArenaAllocator();
        ArenaAllocator(const ArenaAllocator &) = delete;
        ArenaAllocator(ArenaAllocator &&) = delete;
        ~ArenaAllocator();

        ArenaAllocator &operator=(const ArenaAllocator &) = delete;
        ArenaAllocator &operator=(ArenaAllocator &&) = delete;

        size_t capacity() const;

        void addCapacity(Buffer buffer);
        Buffer removeCapacity();

        uint8_t *allocate(size_t size);
        void clear();

    private:
        std::vector<Buffer> buffers_;
        std::vector<uint8_t *> slop_;
        size_t capacity_;
        size_t bufferIndex_;
        size_t bufferTop_;
    };

}

#endif // SNW_ARENA_ALLOCATOR_H
