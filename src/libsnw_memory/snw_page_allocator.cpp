#include "snw_memory.h"

using namespace Snowda;

PageAllocator::PageAllocator(MemoryManager &manager)
    : manager_(manager)
{
}

PageAllocator::~PageAllocator()
{
}

void PageAllocator::addBlock(Block *block)
{
    blocks_.push_back(block);
}

Page *PageAllocator::allocatePage()
{
    // TODO
    abort();
}

void PageAllocator::deallocatePage(Page *page)
{
    // TODO
    abort();
}
