#include "snw_heap.h"

using namespace Snowda;

PageSet::PageSet()
{
    index_[0] = 0;
    index_[1] = 0;
}

bool PageSet::empty() const
{
    return (index_[0] == 0) && (index_[1] == 0);
}

bool PageSet::test(size_t index) const
{
    return (index_[index / 64] & (uint64_t(1) << index % 64)) != 0;
}

void PageSet::set(size_t index)
{
    index_[index / 64] |= (uint64_t(1) << (index % 64));
}

void PageSet::clear(size_t index)
{
    index_[index / 64] &= ~(uint64_t(1) << (index % 64));
}

size_t PageSet::first() const
{
    // FIXME: Use intrinsics
    for (size_t j = 0; j < 2; ++j) {
        for (size_t i = 0; i < 64; ++i) {
            size_t index = (j * 64) + i;
            if (test(index)) {
                return index;
            }
        }
    }

    abort();
}

PageSet PageSet::operator |(const PageSet &rhs) const
{
    PageSet result;
    result.index_[0] = index_[0] | rhs.index_[0];
    result.index_[1] = index_[1] | rhs.index_[1];
    return result;
}
