#ifndef SNW_UTIL_H
#define SNW_UTIL_H

#include <limits>
#include <utility>
#include <type_traits>
#include <iostream>

#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

#if defined(SNW_OS_WIN32)
    #define SNW_ALIGN8 __declspec(align(8))
    __declspec(align(8)) uint8_t error_[sizeof(Error)];
#elif defined(SNW_OS_POSIX)
    #define SNW_ALIGN8 alignas(8)
#endif

#include "snw_fixed_vector.h"
#include "snw_result.h"
#include "snw_view.h"

#endif // SNW_UTIL_H
