#include "snw_heap.h"

using namespace Snowda;

PageAllocator::PageAllocator()
{
}

PageAllocator::~PageAllocator()
{
    // assert(pagesOutstanding_ == 0);

    // for (Block *block: blocks_) {
    //     delete block;
    // }
}

// Page *PageAllocator::allocatePage(PageType pageType, PageOwnerType ownerType, PageOwner owner);
// {
//     if (freePages_.empty()) {
//         grow();
//     }

//     FreePage *freePage = freePages_.back();
//     freePages_.pop();

//     BlockPage *blockPage = freePage->blockPage;
//     BlockPageEntry *blockPageEntry = nullptr; // TODO
//     blockPageEntry->pageType = pageType;
//     blockPageEntry->pageOwnerType = pageOwnerType;
//     blockPageEntry->pageOwner = pageOwner;

//     return reinterpret_cast<Page *>(freePage);
// }

// void PageAllocator::deallocateBufferPage(BufferPage *bufferPage)
// {
//     // TODO
//     abort();
// }

// void PageAllocator::deallocateAllocatorPage(AllocatorPage *allocatorPage)
// {
//     // TODO
//     abort();
// }

// void PageAllocator::deallocatePage(PageType pageType, Page *page)
// {
//     switch (pageType) {
//     case PageType::BufferPage:
//         deallocate(&page->bufferPage);
//         break;
//     case PageType::AllocatorPage:
//         deallocate(&page->allocatorPage);
//         break;
//     default:
//         abort();
//     }
// }

// Block *PageAllocator::findBlock(Page *page) const
// {
//     // TODO: Binary search blocks_
//     abort();
// }

// void PageAllocator::grow()
// {
//     // FIXME: block needs to be page aligned
//     Block *block = new Block;
//     BlockPage &blockPage = block->blockPage;
//     for (BlockPageEntry &blockPageEntry: blocks_) {
//         blockPageEntry.pageType = PageType::FreePage;
//     }
//     for (Page &page: block->pages) {
//         FreePage &freePage = page.freePage;
//         freePage.blockPage = &blockPage;
//     }

//     FreePage *freePageBegin = reinterpret_cast<FreePage *>(blockPage.pages.data());
//     FreePage *freePageEnd = reinterpret_cast<FreePage *>(blockPage.pages.data() + blockPage.pages.size());
//     freePages.insert(freePages.end(), freePageBegin(), freePageEnd());

//     auto it = std::lower_bound(blocks_.begin(), blocks_.end(), block);
//     blocks_.insert(it, block);
// }
