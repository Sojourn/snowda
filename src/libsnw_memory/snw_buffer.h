#ifndef SNW_BUFFER_H
#define SNW_BUFFER_H

namespace Snowda {

    class Buffer {
    public:
        Buffer();
        Buffer(uint8_t *data, size_t size);

        uint8_t *data();
        const uint8_t *data() const;
        size_t size() const;
        explicit operator bool() const;

    private:
        uint8_t *data_;
        size_t size_;
    };
    
}

#endif // SNW_BUFFER_H
