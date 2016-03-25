#include "heap.h"

#include <limits>

using namespace Snowda;

BlockMap::BlockMap()
    : count_(0)
{
    current_.base = std::numeric_limits<uint64_t>::max();
    current_.bitmap = 0;

    for (auto &val: l1_) {
        val = 0;
    }
}

void BlockMap::push(size_t count)
{
    uint64_t id = count_++;
}

void BlockMap::pop(size_t count)
{
    // Not implemented
    abort();
}

uint64_t BlockMap::l1Index(uint64_t id) const
{
    return (id & L1Mask) >> L1Shift;
}

uint64_t BlockMap::l2Index(uint64_t id) const
{
    return (id & L2Mask) >> L2Shift;
}

uint64_t BlockMap::l3Index(uint64_t id) const
{
    return (id & L3Mask) >> L3Shift;
}

BlockRegion BlockMap::scan()
{
}
