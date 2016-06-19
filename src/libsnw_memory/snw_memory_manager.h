#ifndef SNW_MEMORY_MANAGER_H
#define SNW_MEMORY_MANAGER_H

namespace Snowda {

    // FIXME: Rename to MemoryContext
    class MemoryManager {
        friend class PageAllocator;
        friend class ArenaAllocator;
    public:
        MemoryManager();

        PageAllocator &pageAllocator();
        const PageAllocator &pageAllocator() const;

        ArenaAllocator &arenaAllocator();
        const ArenaAllocator &arenaAllocator() const;

    private:
        void allocateRegion();
        void growPageAllocator();
        void growArenaAllocator(ArenaAllocator &arena);

    private:
        PageAllocator pageAllocator_;
        ArenaAllocator arenaAllocator_;
        ChunkAllocator chunkAllocator_;
        std::vector<Region> regions_;
        size_t currentRegionCommitted_;
    };

}

#endif // SNW_MEMORY_MANAGER_H
