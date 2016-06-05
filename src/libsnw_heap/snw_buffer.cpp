#include "snw_heap.h"

using namespace Snowda;

Buffer::Buffer()
    : data_(nullptr)
    , size_(0)
{
}

Buffer::Buffer(uint8_t *data, size_t size)
    : data_(data)
    , size_(size)
{
    assert(data != nullptr);
    assert(size != 0);
}

Buffer::operator bool() const
{
    return data_ != nullptr;
}

uint8_t *Buffer::data()
{
    return data_;
}

const uint8_t *Buffer::data() const
{
    return data_;
}

size_t Buffer::size() const
{
    return size_;
}
