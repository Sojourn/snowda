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
    (void)vaddr;
    return nullptr;
}

VirtualAddress Heap::translate(PhysicalAddress paddr) const
{
    (void)paddr;
    return 0;
}
