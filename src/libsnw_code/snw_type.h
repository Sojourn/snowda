#ifndef SNW_TYPE_H
#define SNW_TYPE_H

namespace Snowda {

    struct Member {
        uint32_t type;
        uint32_t offset;
    };

    struct Type {
        uint32_t typeId;
        uint32_t size;
        uint16_t methodCount;
        uint16_t memberCount;
        uint32_t *methodIds;
        Member *members;
    };
}

#endif // SNW_TYPE_H
