#ifndef SNW_PAGE_H
#define SNW_PAGE_H

namespace Snowda {
    enum {
        pageSize = 4096,
    };

    using Page = std::array<uint8_t, pageSize>;
}

#endif // SNW_PAGE_H
