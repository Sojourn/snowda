#include "snw_heap.h"

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

	static const size_t regionAlignment = 4096;
	static const size_t regionSize = 2 << 20;
}

Region::Region()
    : base_(nullptr)
{
    base_ = allocateVirtualMemory(regionSize);
    assert(base_ != nullptr);

    bool sts = modifyVirtualMemory(base_, regionSize);
    assert(sts);
}

Region::~Region()
{
    if (base_) {
        freeVirtualMemory(base_, 2 << 20);
        base_ = nullptr;
    }
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
	return regionSize;
}

Region::operator bool() const
{
    return base_ != nullptr;
}
