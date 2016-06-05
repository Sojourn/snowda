#include "snw_memory.h"

using namespace Snowda;

namespace {
    static const size_t minBufferSize = sizeof(Page);
}

ArenaFrame::ArenaFrame(ArenaAllocator &arena)
    : arena_(arena)
    , prev_(nullptr)
    , bufferIndex_(0)
    , bufferTop_(0)
{
    arena_.pushFrame(this);
}

ArenaFrame::~ArenaFrame()
{
    arena_.popFrame(this);
}

ArenaAllocator::ArenaAllocator(MemoryManager &manager)
    : manager_(manager)
    , topFrame_(nullptr)
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
    assert(minBufferSize <= buffer.size());

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
    assert(size <= minBufferSize);

    size_t alignedSize = align(size, sizeof(uint64_t));

    for (;;) {
        if (bufferIndex_ == buffers_.size()) {
            manager_.growPageAllocator();
            assert(bufferIndex_ < buffers_.size());
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
    bufferIndex_ = 0;
    bufferTop_ = 0;
}

void ArenaAllocator::pushFrame(ArenaFrame *frame)
{
    frame->prev_ = topFrame_;
    frame->bufferIndex_ = bufferIndex_;
    frame->bufferTop_ = bufferTop_;
    topFrame_ = frame;
}

void ArenaAllocator::popFrame(ArenaFrame *frame)
{
    topFrame_ = frame->prev_;
    bufferIndex_ = frame->bufferIndex_;
    bufferTop_ = frame->bufferTop_;
}
