#ifndef SNW_ARENA_ALLOCATOR_H
#define SNW_ARENA_ALLOCATOR_H

namespace Snowda {

    class ArenaFrame {
        friend class ArenaAllocator;
    public:
        ArenaFrame(ArenaAllocator &arena);
        ArenaFrame(const ArenaFrame &) = delete;
        ArenaFrame(ArenaFrame &&) = delete;
        ~ArenaFrame();

        ArenaFrame &operator=(const ArenaFrame &) = delete;
        ArenaFrame &operator=(ArenaFrame &&) = delete;

    private:
        ArenaAllocator &arena_;
        ArenaFrame *prev_;
        size_t bufferIndex_;
        size_t bufferTop_;
    };

    class ArenaAllocator {
        friend class ArenaFrame;
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
        void pushFrame(ArenaFrame *frame);
        void popFrame(ArenaFrame *frame);

    private:
        MemoryManager &manager_;
        std::vector<Buffer> buffers_;
        ArenaFrame *topFrame_;
        size_t capacity_;
        size_t bufferIndex_;
        size_t bufferTop_;
    };

}

#endif // SNW_ARENA_ALLOCATOR_H
