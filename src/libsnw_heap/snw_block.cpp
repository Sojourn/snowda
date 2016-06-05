#include "snw_heap.h"

using namespace Snowda;

void Block::clear()
{
    for (BlockEntry &entry: entries_) {
        entry.owner = 0;
        entry.flags = 0;
    }
}

BlockEntry &Block::entry(size_t index)
{
    assert(index < entries_.size());

    return entries_[index];
}

const BlockEntry &Block::entry(size_t index) const
{
    assert(index < entries_.size());

    return entries_[index];
}

BlockEntry &Block::entry(Page &page)
{
    return entries_[index(page)];
}

const BlockEntry &Block::entry(const Page &page) const
{
    return entries_[index(page)];
}

Page &Block::page(size_t index)
{
    assert(index < entries_.size());

    uintptr_t lower = reinterpret_cast<uintptr_t>(this);
    uintptr_t upper = lower + ((index + 1) * sizeof(Page));

    return *reinterpret_cast<Page *>(upper);
}

const Page &Block::page(size_t index) const
{
    assert(index < entries_.size());

    uintptr_t lower = reinterpret_cast<uintptr_t>(this);
    uintptr_t upper = lower + ((index + 1) * sizeof(Page));

    return *reinterpret_cast<Page *>(upper);
}

size_t Block::pageCount() const
{
    return entries_.size();
}

size_t Block::index(const Page &page) const
{
    uintptr_t lower = reinterpret_cast<uintptr_t>(this);
    uintptr_t upper = reinterpret_cast<uintptr_t>(&page);

    assert(lower < upper);
    assert(upper <= (lower + (2 << 20)));

    size_t result = ((upper - lower) / 4096) - 1;
    assert(result < 512);

    return result;
}
