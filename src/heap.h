#ifndef SNOWDA_HEAP_H
#define SNOWDA_HEAP_H

#include <cstdint>

#include <vector>
#include <memory>
#include <array>

namespace Snowda {

    struct BlockRegion {
        uint64_t base;
        uint64_t bitmap;
    };

    class BlockMap {
    public:
        BlockMap();

        void push(size_t count);
        void pop(size_t count);
        BlockRegion scan();

    private:
        uint64_t l1Index(uint64_t id) const;
        uint64_t l2Index(uint64_t id) const;
        uint64_t l3Index(uint64_t id) const;

    private:
        enum {
            L3Bits  = 20,
            L3Mask  = L3Bits - 1,
            L3Shift = 0,
            L2Bits  = 16,
            L2Mask  = L2Bits - 1,
            L2Shift = L3Shift + L3Bits,
            L1Bits  = 12,
            L1Mask  = L1Bits - 1,
            L1Shift = L2Shift + L2Bits,
        };

        using L1 = std::array<uint64_t, (1 << L1Bits) / sizeof(uint64_t)>;
        using L2 = std::array<uint64_t, (1 << L2Bits) / sizeof(uint64_t)>;

        uint64_t        count_;
        BlockRegion     current_;
        std::vector<L2> l2s_;
        L1              l1_;
    };

    class Heap {
    public:
        Heap(size_t size);

        uint8_t *allocate();
        void deallocate(uint8_t *ptr);

    private:
        BlockMap                   map_;
        std::unique_ptr<uint8_t[]> memory_;
    };

}

#endif // SNOWDA_HEAP_H
