#include "snw_memory.h"

using namespace Snowda;

HeapAllocation::HeapAllocation()
    : addr_(nullptr)
    , vaddr_(0)
{
}

HeapAllocation::HeapAllocation(uint8_t *addr, VirtualAddress vaddr)
    : addr_(addr)
    , vaddr_(vaddr)
{
}

uint8_t *HeapAllocation::addr() const
{
    return addr_;
}

VirtualAddress HeapAllocation::vaddr() const
{
    return vaddr_;
}

HeapAllocation::operator bool() const
{
    return addr_ != nullptr;
}

Heap::Heap(MemoryManager &manager)
    : manager_(manager)
    , pageIndex_(0)
    , pageTop_(0)
{
    pages_[0] = manager.pageAllocator().allocatePage();
    pageSet_.set(0);
}

HeapAllocation Heap::allocate(size_t size)
{
    assert(size <= sizeof(Page));

    size_t alignedSize = align(size, 16);
    if (sizeof(Page) - pageTop_) {
        auto clearedPage = pageSet_.firstCleared();
        if (std::get<1>(clearedPage)) {
            return HeapAllocation();
        }

        Page *page = pages_[std::get<0>(clearedPage)];
        pageSet_.set(std::get<0>(clearedPage));

        VirtualAddress vaddr = static_cast<VirtualAddress>(std::get<0>(clearedPage)) << 8;
        HeapAllocation allocation(page->data(), vaddr);
        pageTop_ = static_cast<uint16_t>(alignedSize);
        return allocation;
    }
    else {
        VirtualAddress vaddr = (pageIndex_ << 8) | (pageTop_ >> 4);
        HeapAllocation allocation(pages_[pageIndex_]->data() + pageTop_, vaddr);
        pageTop_ += static_cast<uint16_t>(alignedSize);
        return allocation;
    }
}

uint8_t *Heap::deref(VirtualAddress vaddr)
{
    uint8_t pageIndex = vaddr >> 8;
    uint8_t slotIndex = vaddr & 255;
    Page *page = pages_[pageIndex];
    return page->data() + (slotIndex << 4);
}
