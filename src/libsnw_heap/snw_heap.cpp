#include "snw_heap.h"

using namespace Snowda;

Heap::Heap(PagePool &pagePool)
    : pagePool_(pagePool)
{
}

std::tuple<PhysicalAddress, VirtualAddress, bool> allocate(uint8_t size)
{
    return std::make_tuple(nullptr, 0, false);
}

void Heap::deallocate(PhysicalAddress paddr)
{
}

void Heap::deallocate(VirtualAddress vaddr)
{
}

PhysicalAddress Heap::translate(VirtualAddress vaddr) const
{
    Page *page = pages_[vaddr >> 8].get();
    uint8_t *pageAddr = reinterpret_cast<uint8_t *>(page);
    return pageAddr + ((vaddr & 255) << 9);
}

VirtualAddress Heap::translate(PhysicalAddress paddr) const
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(paddr);
    uintptr_t pageAddr = addr & (sizeof(Page) - 1);
    Page *page = reinterpret_cast<Page *>(pageAddr);
    VirtualAddress shiftedPageIndex = static_cast<VirtualAddress>(page->header.index) << 8;
    return shiftedPageIndex + ((addr >> 9) & 255);
}
