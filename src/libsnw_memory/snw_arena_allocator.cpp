#include "snw_memory.h"

using namespace Snowda;

ArenaAllocator::ArenaAllocator(MemoryManager &manager)
    : manager_(manager)
    , capacity_(0)
    , bufferIndex_(0)
    , bufferTop_(0)
{
}

ArenaAllocator::~ArenaAllocator()
{
    clear();
}

size_t ArenaAllocator::capacity() const
{
    return capacity_;
}

void ArenaAllocator::addBuffer(Buffer buffer)
{
    assert(buffer);

    buffers_.push_back(buffer);
    capacity_ += buffer.size();
}

Buffer ArenaAllocator::removeBuffer()
{
    if (bufferIndex_ < buffers_.size()) {
        Buffer result = std::move(buffers_.back());
        buffers_.pop_back();
        return std::move(result);
    }

    return Buffer();
}

uint8_t *ArenaAllocator::allocate(size_t size)
{
    size_t alignedSize = align(size, sizeof(uint64_t));

    for (;;) {
        if (bufferIndex_ == buffers_.size()) {
            return new uint8_t[size];
        }

        Buffer &buffer = buffers_[bufferIndex_];
        if (alignedSize <= (buffer.size() - bufferTop_)) {
            uint8_t *addr = buffer.data() + bufferTop_;
            bufferTop_ += alignedSize;
            return addr;
        }
    }
}

void ArenaAllocator::clear()
{
    for (uint8_t *addr: slop_) {
        delete [] addr;
    }
    slop_.clear();

    bufferIndex_ = 0;
    bufferTop_ = 0;
}
