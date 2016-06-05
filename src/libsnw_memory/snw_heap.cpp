#include "snw_memory.h"

using namespace Snowda;

Heap::Heap(PageAllocator &pageAllocator)
    : pageAllocator_(pageAllocator)
{
}

Heap::~Heap()
{
}
