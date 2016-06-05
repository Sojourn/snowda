#include "snw_memory.h"

#if defined(SNW_OS_WIN32)
#include "Windows.h"
#elif defined(SNW_OS_UNIX)
#error "Not implemented"
#endif

using namespace Snowda;

namespace {
    uint8_t *allocateVirtualMemory(size_t size);
    bool freeVirtualMemory(uint8_t *addr, size_t size);
    bool modifyVirtualMemory(uint8_t *addr, size_t size);

#if defined(SNW_OS_WIN32)
    uint8_t *allocateVirtualMemory(size_t size)
    {
        auto result = VirtualAlloc(nullptr, size, MEM_RESERVE, PAGE_NOACCESS);
        return reinterpret_cast<uint8_t *>(result);
    }

    bool freeVirtualMemory(uint8_t *addr, size_t size)
    {
        return VirtualFree(addr, 0, MEM_RELEASE) == TRUE;
    }

    bool modifyVirtualMemory(uint8_t *addr, size_t size)
    {
        auto result = VirtualAlloc(addr, size, MEM_COMMIT, PAGE_READWRITE);
        return reinterpret_cast<uint8_t *>(result) == addr;
    }
#elif defined(SNW_OS_UNIX)
#error "Not implemented"
#endif
}

Region::Region(size_t size)
    : base_(nullptr)
    , size_(size)
    , committed_(0)
{
    assert(size > 0);
    assert((size & (sizeof(Page) - 1)) == 0);

    base_ = allocateVirtualMemory(size);
    assert(base_ != nullptr);
}

Region::Region(Region &&other)
    : base_(other.base_)
    , size_(other.size_)
    , committed_(other.committed_)
{
    other.base_ = nullptr;
    other.size_ = 0;
    other.committed_ = 0;
}

Region::~Region()
{
    if (base_) {
        freeVirtualMemory(base_, size_);
        base_ = nullptr;
    }
}

Region &Region::operator=(Region &&rhs)
{
    if (this != &rhs) {
        if (*this) {
            this->~Region();
        }

        base_ = rhs.base_;
        size_ = rhs.size_;
        committed_ = rhs.committed_;

        rhs.base_ = nullptr;
        rhs.size_ = 0;
        rhs.committed_ = 0;
    }

    return *this;
}

void Region::commit(size_t size)
{
    assert(*this);
    assert(size > 0);
    assert((size & (sizeof(Page) - 1)) == 0);
    assert((committed_ + size) <= size_);

    bool sts = modifyVirtualMemory(base_ + committed_, size);
    assert(sts);

    committed_ += size;
}

size_t Region::committed() const
{
    return committed_;
}

uint8_t *Region::data()
{
    return base_;
}

const uint8_t *Region::data() const
{
    return base_;
}

size_t Region::size() const
{
	return size_;
}

Region::operator bool() const
{
    return base_ != nullptr;
}
