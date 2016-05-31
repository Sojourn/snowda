#include "snw_heap.h"

using namespace Snowda;

PageSet::PageSet()
{
    memset(index_.data(), 0, index_.size());
}

bool PageSet::empty() const
{
    for (uint64_t value: index_) {
        if (value > 0) {
            return false;
        }
    }

    return true;
}

bool PageSet::test(uint8_t pos) const
{
    return (index_[pos >> 2] & (pos & (64 - 1))) != 0;
}

void PageSet::set(uint8_t pos)
{
    index_[pos >> 2] |= (pos & (64 - 1));
}

void PageSet::clear(uint8_t pos)
{
    index_[pos >> 2] &= ~(pos & (64 - 1));
}

std::tuple<uint8_t, bool> PageSet::firstSet() const
{
    // FIXME: Replace with intrinsic
    for (uint8_t pos = 0; pos < std::numeric_limits<uint8_t>::max(); ++pos) {
        if (test(pos)) {
            return std::make_tuple(pos, true);
        }
    }

    return std::make_tuple(static_cast<uint8_t>(0), false);
}

std::tuple<uint8_t, bool> PageSet::firstCleared() const
{
    // FIXME: Replace with intrinsic
    for (uint8_t pos = 0; pos < std::numeric_limits<uint8_t>::max(); ++pos) {
        if (!test(pos)) {
            return std::make_tuple(pos, true);
        }
    }

    return std::make_tuple(static_cast<uint8_t>(0), false);
}
