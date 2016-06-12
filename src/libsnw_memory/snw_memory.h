#ifndef SNW_MEMORY_H
#define SNW_MEMORY_H

#include <array>
#include <memory>
#include <vector>

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

#include "snw_util.h"

namespace Snowda {
    class Buffer;
    class Page;
    class Block;
    class BlockEntry;
    class Region;
    class PageSet;
    class PageAllocator;
    class ArenaAllocator;
    class ChunkAllocator;
    class Heap;
    class MemoryManager;
}

#include "snw_memory_util.h"
#include "snw_buffer.h"
#include "snw_page.h"
#include "snw_block.h"
#include "snw_region.h"
#include "snw_page_set.h"
#include "snw_page_allocator.h"
#include "snw_arena_allocator.h"
#include "snw_chunk_allocator.h"
#include "snw_heap.h"
#include "snw_memory_manager.h"

#endif // SNW_MEMORY_H
