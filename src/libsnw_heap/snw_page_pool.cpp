#include "snw_heap.h"

using namespace Snowda;


PagePool::PagePool()
{
}

PagePool::~PagePool()
{
    assert(pagesOutstanding_ == 0);

    // FIXME: Delete all pages
}

Page *PagePool::allocate()
{
    return nullptr;
}

void PagePool::deallocate(Page *page)
{
}

void PagePool::grow()
{
#ifdef SNW_OS_UNIX
#error "Not implemented"
#else
    void *addr = 0;
#endif
}
