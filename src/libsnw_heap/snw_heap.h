#ifndef SNW_HEAP_H
#define SNW_HEAP_H

#include <array>
#include <memory>
#include <vector>

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>


#include "snw_util.h"

#include "snw_buffer.h"
#include "snw_page.h"
#include "snw_block.h"
#include "snw_region.h"
#include "snw_page_set.h"
#include "snw_page_allocator.h"
#include "snw_arena_allocator.h"

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
