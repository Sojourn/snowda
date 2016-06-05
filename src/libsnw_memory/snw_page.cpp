#include "snw_memory.h"

using namespace Snowda;

Block &Page::block()
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(this);
    addr &= ~((2 << 20) - 1);
    return *reinterpret_cast<Block *>(addr);
}

const Block &Page::block() const
{
    uintptr_t addr = reinterpret_cast<uintptr_t>(this);
    addr &= ~((2 << 20) - 1);
    return *reinterpret_cast<const Block *>(addr);
}

BlockEntry &Page::blockEntry()
{
    return block().entry(*this);
}

const BlockEntry &Page::blockEntry() const
{
    return block().entry(*this);
}

size_t Page::size() const
{
    return data_.size();
}

uint8_t *Page::data()
{
    return data_.data();
}

const uint8_t *Page::data() const
{
    return data_.data();
}
