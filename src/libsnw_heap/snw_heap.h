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

#include "snw_page.h"
#include "snw_page_set.h"
#include "snw_page_pool.h"

namespace Snowda {

    using PhysicalAddress = uint8_t *;
    using VirtualAddress = uint16_t;

    struct ObjectHeader {
        uint8_t size;
        uint8_t flags;
        uint16_t reserved;
        uint32_t type;
    };

    class Heap {
    public:
        Heap(PagePool &pagePool);

        std::tuple<PhysicalAddress, VirtualAddress, bool> allocate(uint8_t size);
        void deallocate(PhysicalAddress paddr);
        void deallocate(VirtualAddress vaddr);

        PhysicalAddress translate(VirtualAddress vaddr) const;
        VirtualAddress translate(PhysicalAddress paddr) const;

    private:
        PagePool &pagePool_;
        std::vector<Page> pages_;
        PageSet pageSet_;
    };

}

#endif // SNW_HEAP_H
