#ifndef SNW_BUFFER_PAGE_H
#define SNW_BUFFER_PAGE_H

namespace Snowda {

    struct BufferPage {
        uint8_t data[4096];
    };
    static_assert(sizeof(BufferPage) == 4096, "Bad buffer page layout");

}

#endif // SNW_BUFFER_PAGE_H
