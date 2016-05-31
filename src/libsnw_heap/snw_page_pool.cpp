#include "snw_heap.h"

using namespace Snowda;


void PagePool::PageDeleter::operator()(Page *page)
{
    void *addr = reinterpret_cast<void *>(page);
}

PagePool::PagePool()
{
}

PagePool::~PagePool()
{
    assert(pagesOutstanding_ == 0);
}

std::tuple<PagePtr, bool> PagePool::acquire()
{
    return std::make_tuple(nullptr, false);
}

void PagePool::release(PagePtr page)
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
