#ifndef SNW_MEMORY_UTIL_H
#define SNW_MEMORY_UTIL_H

namespace Snowda {

    template<typename T>
    T align(T value, size_t alignment)
    {
        T alignedValue = (value + alignment - 1) & ~(alignment - 1);
        assert((alignedValue & (alignment - 1)) == 0);
        return alignedValue;
    }

    template<typename T>
	bool isAligned(T value, size_t alignment);

    template<typename T>
    bool isAligned(T *value, size_t alignment)
    {
        assert(alignment > 0);
        return (reinterpret_cast<uintptr_t>(value) & (alignment - 1)) == 0;
    }

    template<typename T>
    bool isAligned(T value, size_t alignment)
    {
        assert(alignment > 0);
        return (value & (alignment - 1)) == 0;
    }

}

#endif // SNW_MEMORY_UTIL_H
