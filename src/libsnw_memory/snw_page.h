#ifndef SNW_PAGE_H
#define SNW_PAGE_H

namespace Snowda {

    class Page {
    public:
        Block &block();
        const Block &block() const;
        BlockEntry &blockEntry();
        const BlockEntry &blockEntry() const;

        size_t size() const;
        uint8_t *data();
        const uint8_t *data() const;

    private:
        std::array<uint8_t, 4096> data_;
    };

}

#endif // SNW_PAGE_H
