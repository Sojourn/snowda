#ifndef SNW_PAGE_POOL_H
#define SNW_PAGE_POOL_H

namespace Snowda {

    class PageAllocator {
    public:
        PageAllocator();
        ~PageAllocator();

        // Page *allocatePage(PageOwnerType ownerType, uint32_t ownerKey);
        // void deallocatePage(Page *page);

    private:
        // Block *findBlock(Page *page) const;
        // void grow();

    private:
        // std::vector<FreePage *> freePages_;
        // std::vector<Block *> blocks_;
        // size_t pagesOutstanding_;
    };

}

#endif // SNW_PAGE_POOL_H
