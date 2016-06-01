#ifndef SNW_PAGE_POOL_H
#define SNW_PAGE_POOL_H

namespace Snowda {

    class PagePool {
    public:
        PagePool();
        ~PagePool();

        Page *allocate();
        void deallocate(Page *page);

    private:
        void grow();

    private:
        std::vector<Page *> pages_;
        size_t pagesOutstanding_;
    };

}

#endif // SNW_PAGE_POOL_H
