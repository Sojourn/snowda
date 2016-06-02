#ifndef SNW_PAGE_POOL_H
#define SNW_PAGE_POOL_H

namespace Snowda {

    class PageAllocator {
    public:
        PageAllocator();
        ~PageAllocator();

        Page *allocateBufferPage(PageOwnerType ownerType, PageOwner owner);
        Page *allocateAllocatorPage(PageOwnerType ownerType, PageOwner owner);
        Page *allocatePage(PageType pageType, PageOwnerType ownerType, PageOwner owner);

        void deallocateBufferPage(BufferPage *bufferPage);
        void deallocateAllocatorPage(AllocatorPage *allocatorPage);
        void deallocatePage(PageType pageType, Page *page);

    private:
        Block *findBlock(Page *page) const;
        void grow();

    private:
        std::vector<FreePage *> freePages_;
        std::vector<Block *> blocks_;
        size_t pagesOutstanding_;
    };

}

#endif // SNW_PAGE_POOL_H
