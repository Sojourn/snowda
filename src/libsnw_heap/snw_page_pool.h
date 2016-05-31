#ifndef SNW_PAGE_POOL_H
#define SNW_PAGE_POOL_H

namespace Snowda {

    class PagePool {
    public:
        struct PageDeleter {
            void operator()(Page *page);
        };

        using PagePtr = std::unique_ptr<Page, PageDeleter>;

    public:
        PagePool();
        ~PagePool();

        std::tuple<PagePtr, bool> acquire();
        void release(PagePtr page);

    private:
        void grow();

    private:
        std::vector<PagePtr> pages_;
        size_t pagesOutstanding_;
    };
    using PagePtr = PagePool::PagePtr;

}

#endif // SNW_PAGE_POOL_H
