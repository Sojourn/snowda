#include "snw_memory.h"

using namespace Snowda;

PageAllocator::PageAllocator(MemoryManager &manager)
    : manager_(manager)
{
}

void PageAllocator::addBlock(Block *block)
{
    blocks_.push_back(block);
    for (size_t i = 0; i < block->pageCount(); ++i) {
        pages_.push_back(&block->page(i));
    }
}

Page *PageAllocator::allocatePage()
{
    if (pages_.empty()) {
        manager_.growPageAllocator();
        assert(!pages_.empty());
    }

    Page *page = pages_.back();
    pages_.pop_back();
    return page;
}

void PageAllocator::deallocatePage(Page *page)
{
    pages_.push_back(page);
}
