#include "snw_tst.h"

using namespace Snowda;

#include "Windows.h"

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

class Region {
public:
    Region()
        : base_(nullptr)
    {
        assert(size > 0);
        assert((size > 0) && ((size & (4096 - 1)) == 0));
        assert((alignment > 0) && ((alignment & (4096 - 1)) == 0));

        base_ = allocateVirtualMemory(size_ + );
        assert(base_ != nullptr);

        bool sts = modifyVirtualMemory(base_, size_);
        assert(sts);
    }

    ~Region()
    {
        if (base_) {
            freeVirtualMemory(base_, size_);
            base_ = nullptr;
            size_ = 0;
        }
    }

    uint8_t *data()
    {
        return base_;
    }

    const uint8_t *data() const
    {
        return base_;
    }

    size_t size() const
    {
        return size_;
    }

    explicit operator bool() const
    {
        return base_ != nullptr;
    }

private:
    uint8_t *base_;
};

class Buffer {
public:
    Buffer()
        : data_(nullptr)
        , size_(0)
    {
    }

    Buffer(uint8_t *data, size_t size)
        : data_(data)
        , size_(size)
    {
        assert(data != nullptr);
        assert(size != 0);
    }

    Buffer(const Buffer &) = delete;

    Buffer(Buffer &&other)
        : data_(other.data())
        , size_(other.size())
    {
        other = Buffer();
    }

    Buffer &operator=(const Buffer &) = delete;

    Buffer &operator=(Buffer &&rhs)
    {
        if (this != &rhs) {
            data_ = rhs.data();
            size_ = rhs.size();
            rhs = Buffer();
        }

        return *this;
    }

    explicit operator bool() const
    {
        return data_ != nullptr;
    }

    uint8_t *data()
    {
        return data_;
    }

    const uint8_t *data() const
    {
        return data_;
    }

    size_t size() const
    {
        return size_;
    }

private:
    uint8_t *data_;
    size_t size_;
};

class ArenaAllocator {
public:
    ArenaAllocator()
        : capacity_(0)
        , bufferIndex_(0)
        , bufferTop_(0)
    {
    }

    ~ArenaAllocator()
    {
        for (uint8_t *addr: slop_) {
            delete [] addr;
        }
        slop_.clear();
    }

    size_t capacity() const
    {
        return capacity_;
    }

    void addCapacity(Buffer buffer)
    {
        assert(buffer);

        buffers_.push_back(buffer);
        capacity_ += buffer.size();
    }

    Buffer removeCapacity()
    {
        if (bufferIndex_ < buffers_.size()) {
            Buffer result = std::move(buffers_.back());
            buffers_.pop_back();
            return std::move(result);
        }

        return Buffer();
    }

    uint8_t *allocate(size_t size)
    {
        size_t alignedSize = (size + sizeof(uint64_t) - 1) & ~(sizeof(uint64_t) - 1);
        assert((alignedSize > 0) && ((alignedSize & (sizeof(uint64_t) - 1)) == 0));

        for (;;) {
            if (bufferIndex_ == buffers_.size()) {
                return new uint8_t[size];
            }

            Buffer &buffer = buffers_[bufferIndex_];
            if (alignedSize <= (buffer.size() - bufferTop)) {
                uint8_t *addr = buffer.data() + bufferTop_;
                bufferTop_ += alignedSize;
                return addr;
            }
        }
    }

    void reset()
    {
        for (uint8_t *addr: slop_) {
            delete [] addr;
        }
        slop_.clear();

        bufferIndex_ = 0;
        bufferTop_ = 0;
    }

private:
    std::vector<Buffer> buffers_;
    std::vector<uint8_t *> slop_;
    size_t capacity_;
    size_t bufferIndex_;
    size_t bufferTop_;
};

class Page;
class Block;
class Region;

class Region {
public:
};

class Block {
public:
};

class Page {
public:
    Block &block();
    const Block &block() const;

private:
    std::array<uint8_t, 4096> data_;
};

class MemoryManager {
public:
    MemoryManager()
    {
    }

    ArenaAllocator &arena()
    {
        return arena_;
    }

    const ArenaAllocator &arena() const
    {
        return arena_;
    }

    Page *allocatePage(uint32_t owner)
    {
        if (pages_.empty()) {
            // TODO: Allocate pages
            abort();
        }

        Page *page = pages_.back();
        page->setOwner(owner);
        pages_.pop_back();
        return page;
    }

    void freePage(Page *page)
    {
        assert(page != nullptr);

        page->setOwner(0);
        page->setFlags(0);
        pages_.push_back(page);
    }

private:
    enum {
        regionSize = 512 << 20,
        regionAlignment = 4096,
        blockSize = 2 << 20,
        blockAlignment = blockSize
    };

    ArenaAllocator arena_;
    std::vector<Region> regions_;
    std::vector<Page *> pages_;
};

int main(int argc, char **argv) {
    Region region(2 << 20);
    assert(region);

	memset(region.data(), 7, region.size());

    return 0;
}