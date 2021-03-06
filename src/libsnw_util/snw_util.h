#ifndef SNW_UTIL_H
#define SNW_UTIL_H

#include <tuple>
#include <limits>
#include <utility>
#include <type_traits>
#include <array>
#include <iterator>
#include <iostream>

#include <cstdlib>
#include <cstring>
#include <cstddef>
#include <cstdint>
#include <cassert>

#if defined(SNW_OS_WIN32)
    #define SNW_ALIGN8 __declspec(align(8))
#elif defined(SNW_OS_POSIX)
    #define SNW_ALIGN8 alignas(8)
#endif

#include "snw_fixed_vector.h"
#include "snw_intrusive_list.h"
#include "snw_result.h"
#include "snw_view.h"
#include "snw_bit_tree.h"
#include "snw_bit_tree_inline.h"

#endif // SNW_UTIL_H
