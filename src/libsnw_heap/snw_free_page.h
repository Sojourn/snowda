#ifndef SNW_FREE_PAGE_H
#define SNW_FREE_PAGE_H

namespace Snowda {

    struct FreePage {
        BlockPage *block;
    };
    static_assert(sizeof(FreePage) == 4096, "Bad free page layout");
}

#endif // SNW_FREE_PAGE_H
