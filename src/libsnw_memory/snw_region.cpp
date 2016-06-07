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
    bool modifyVirtualMemory(uint8_t *addr, size_t size, RegionProtection protection);

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

    bool modifyVirtualMemory(uint8_t *addr, size_t size, RegionProtection protection)
    {
        DWORD translatedProtection = 0;
        switch (protection) {
        case RegionProtection::None:
            translatedProtection = PAGE_NOACCESS;
            break;
        case RegionProtection::Read:
			translatedProtection = PAGE_READONLY;
            break;
        case RegionProtection::ReadWrite:
            translatedProtection = PAGE_READWRITE;
            break;
        case RegionProtection::ReadExecute:
			translatedProtection = PAGE_EXECUTE_READ;
            break;
        default:
            abort();
        }

        auto result = VirtualAlloc(addr, size, MEM_COMMIT, translatedProtection);
        return reinterpret_cast<uint8_t *>(result) == addr;
    }
#elif defined(SNW_OS_UNIX)
#error "Not implemented"
#endif
}

Region::Region()
    : base_(nullptr)
    , size_(0)
{
}

Region::Region(size_t size)
    : base_(nullptr)
    , size_(size)
{
    assert(size > 0);
    assert(isAligned(size, sizeof(Page)));
    assert((size & (sizeof(Page) - 1)) == 0);

    base_ = allocateVirtualMemory(size);
    assert(base_ != nullptr);
}

Region::Region(Region &&other)
    : base_(other.base_)
    , size_(other.size_)
{
    other.base_ = nullptr;
    other.size_ = 0;
}

Region::~Region()
{
    if (*this) {
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

        rhs.base_ = nullptr;
        rhs.size_ = 0;
    }

    return *this;
}

void Region::modify(RegionProtection protection)
{
    modify(0, size_, protection);
}

void Region::modify(size_t offset, size_t size, RegionProtection protection)
{
    assert(*this);
    assert(isAligned(offset, sizeof(Page)));
    assert(size > 0);
    assert(isAligned(size, sizeof(Page)));
    assert((offset + size) <= size_);

    bool sts = modifyVirtualMemory(base_ + offset, size, protection);
    assert(sts);
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
