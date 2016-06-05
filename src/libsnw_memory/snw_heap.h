#ifndef SNW_HEAP_H
#define SNW_HEAP_H

namespace Snowda {

    using VirtualAddress = uint16_t;

    class HeapAllocation {
    public:
        HeapAllocation();
        HeapAllocation(uint8_t *addr, VirtualAddress vaddr);

        uint8_t *addr() const;
        VirtualAddress vaddr() const;

        explicit operator bool() const;

    private:
        uint8_t *addr_;
        VirtualAddress vaddr_;
    };

    class Heap {
    public:
        Heap(MemoryManager &manager);

        HeapAllocation allocate(size_t size);
        uint8_t *deref(VirtualAddress vaddr);

    private:
        MemoryManager &manager_;
        uint8_t pageIndex_;
        uint16_t pageTop_;
        PageSet pageSet_;
        std::array<Page *, 256> pages_;
    };

}

#endif // SNW_HEAP_H
