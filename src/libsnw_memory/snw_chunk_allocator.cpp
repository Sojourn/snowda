#include "snw_memory.h"

using namespace Snowda;

ChunkAllocator::ChunkAllocator(MemoryManager &manager)
    : manager_(manager)
{
}

ChunkAllocator::~ChunkAllocator()
{
    for (auto page: pages_) {
        manager_.pageAllocator().deallocatePage(page);
    }
}

uint8_t *ChunkAllocator::allocate(size_t size)
{
    assert((minChunkSize < size) && (size <= maxChunkSize));

    const size_t chunkListIndex = findChunkList(size);
    const size_t chunkShift = chunkListIndex + minChunkShift;
    const size_t chunkSize = static_cast<size_t>(1) << chunkShift;

    ChunkList &chunkList = chunkLists_[chunkListIndex];
    if (chunkList.empty()) {
        Page *page = manager_.pageAllocator().allocatePage();
        for (size_t i = chunkSize; i < sizeof(Page); i += chunkSize) {
            chunkList.push_back(page->data() + i);
        }
    }
    assert(!chunkList.empty());

    uint8_t *addr = chunkList.back();
    chunkList.pop_back();
    return addr;
}

void ChunkAllocator::deallocate(uint8_t *addr, size_t size)
{
    assert(addr != nullptr);
    assert((minChunkSize < size) && (size <= maxChunkSize));

    const size_t chunkListIndex = findChunkList(size);
    const size_t chunkShift = chunkListIndex + minChunkShift;
    const size_t chunkSize = static_cast<size_t>(1) << chunkShift;

    ChunkList &chunkList = chunkLists_[chunkListIndex];
    chunkList.push_back(addr);
}

size_t ChunkAllocator::findChunkList(size_t size) const
{
    for (size_t i = minChunkShift; i <= maxChunkShift; ++i) {
        if (size <= (static_cast<size_t>(1) << i)) {
            return i - minChunkShift;
        }
    }

    abort();
}
