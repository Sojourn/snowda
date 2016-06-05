#ifndef SNW_HEAP_H
#define SNW_HEAP_H

namespace Snowda {

    class Heap {
    public:
        Heap(PageAllocator &pageAllocator);
        ~Heap();

    private:
        PageAllocator &pageAllocator_;
        std::vector<Page *> pages_;
        size_t currentPageIndex_;
        PageSet pageSet_;
    };

}

#endif // SNW_HEAP_H
