#include "snw_memory.h"

using namespace Snowda;

namespace {
    static const size_t regionSize = (128 << 20);
    static const size_t blockSize = (2 << 20);
    static const size_t blockAlign = (2 << 20);
}

MemoryManager::MemoryManager()
    : pageAllocator_(*this)
    , arenaAllocator_(*this)
{
	allocateRegion();
}

PageAllocator &MemoryManager::pageAllocator()
{
    return pageAllocator_;
}

const PageAllocator &MemoryManager::pageAllocator() const
{
    return pageAllocator_;
}

ArenaAllocator &MemoryManager::arenaAllocator()
{
    return arenaAllocator_;
}

const ArenaAllocator &MemoryManager::arenaAllocator() const
{
    return arenaAllocator_;
}

void MemoryManager::allocateRegion()
{
    regions_.push_back(Region(regionSize));
    Region &region = regions_.back();

    uintptr_t baseAddr = reinterpret_cast<uintptr_t>(region.data());
    uintptr_t blockAddr = align(baseAddr, blockAlign);

    region.commit((blockAddr - baseAddr) + blockSize);

    Buffer buffer(reinterpret_cast<uint8_t *>(baseAddr), blockAddr - baseAddr);
    if (buffer) {
        arenaAllocator_.addBuffer(buffer);
    }

    Block *block = reinterpret_cast<Block *>(blockAddr);
    new (block) Block;
    pageAllocator_.addBlock(block);
}

void MemoryManager::growPageAllocator()
{
    regions_.push_back(Region(regionSize));
    Region &region = regions_.back();
    if (region.committed() == region.size()) {
        allocateRegion();
    }
    else {
        uintptr_t baseAddr = reinterpret_cast<uintptr_t>(region.data());
        uintptr_t blockAddr = baseAddr + region.committed();
        region.commit(blockSize);

        Block *block = reinterpret_cast<Block *>(blockAddr);
        new (block) Block;
		pageAllocator_.addBlock(block);
    }
}

void MemoryManager::growArenaAllocator()
{
    Page *page = pageAllocator_.allocatePage();
	page->blockEntry().setUse(PageUse::Arena);

    Buffer buffer(page->data(), page->size());
    arenaAllocator_.addBuffer(buffer);
}
