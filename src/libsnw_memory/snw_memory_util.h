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

}

#endif // SNW_MEMORY_UTIL_H
