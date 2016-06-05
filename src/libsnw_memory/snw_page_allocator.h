#ifndef SNW_PAGE_POOL_H
#define SNW_PAGE_POOL_H

namespace Snowda {

    class PageAllocator {
    public:
        PageAllocator(MemoryManager &manager);
        ~PageAllocator();

        Page *allocatePage();
        void deallocatePage(Page *page);

        void addBlock(Block *block);

    private:
        Block *findBlock(Page *page) const;

    private:
        MemoryManager &manager_;
        std::vector<Page *> pages_;
        std::vector<Block *> blocks_;
    };

}

#endif // SNW_PAGE_POOL_H
