#include "snw_util.h"

using namespace Snowda;

#include <array>
#include <iostream>

#include <type_traits>

#include <cstdint>
#include <cstddef>
#include <cassert>

template<int level, int target, int... values>
struct FindHelper;

template<int level, int target, int value, int... values>
struct FindHelper<level, target, value, values...> {
    enum {
        value = (value == target) ? level : FindHelper<level + 1, target, values...>::value
    };
};

template<int level, int target, int value>
struct FindHelper<level, target, value> {
    enum {
        value = (value == target) ? level : -1
    };
};

template<int level, int target>
struct FindHelper<level, target> {
    enum {
        value = -1
    };
};

template<int target, int... values>
using Find = FindHelper<0, target, values...>;

static_assert(Find<0>::value == -1, "");
static_assert(Find<0, 0, 1, 2>::value == 0, "");
static_assert(Find<1, 0, 1, 2>::value == 1, "");
static_assert(Find<2, 0, 1, 2>::value == 2, "");
static_assert(Find<3, 0, 1, 2>::value == -1, "");

template<int... values>
struct Set {
    template<int value>
    struct Insert {
        using type = Set<values..., value>;
    };

    template<int value>
    struct Contains {
        enum {
            value = Find<value, values...>::value >= 0 ? 1 : 0
        };
    };
};

static_assert(std::is_same<Set<0>, Set<>::Insert<0>::type>::value, "");
static_assert(std::is_same<Set<0, 1>, Set<0>::Insert<1>::type>::value, "");

static_assert(Set<0, 1, 2>::Contains<0>::value, "");
static_assert(Set<0, 1, 2>::Contains<1>::value, "");
static_assert(Set<0, 1, 2>::Contains<2>::value, "");
static_assert(!Set<0, 1, 2>::Contains<3>::value, "");

template<int key>
struct KeyHash {
    enum {
        value = ((key >> 16) ^ key) * 0x45d9f3b,
    };
};

template<typename Set, int capacity, int position, int offset>
struct ProbeHelper {
    enum {
        slot_ = (position + offset) % capacity,
        value = Set::Contains<slot_>::value ?
            ProbeHelper<Set, capacity, position, offset + 1>::value :
            slot_,
    };
};

template<typename Set, int capacity, int position>
struct ProbeHelper<Set, capacity, position, capacity> {
    enum {
        value = -1
    };
};

template<typename Set, int capacity, int position>
using Probe = ProbeHelper<Set, capacity, position, 0>;

static_assert(Probe<Set<>, 3, 0>::value == 0, "");
static_assert(Probe<Set<0>, 3, 0>::value == 1, "");
static_assert(Probe<Set<0, 1>, 3, 0>::value == 2, "");
static_assert(Probe<Set<0, 1, 2>, 3, 0>::value == -1, "");

static_assert(Probe<Set<>, 3, 1>::value == 1, "");
static_assert(Probe<Set<1>, 3, 1>::value == 2, "");
static_assert(Probe<Set<1, 2>, 3, 1>::value == 0, "");
static_assert(Probe<Set<1, 2, 0>, 3, 1>::value == -1, "");

static_assert(Probe<Set<>, 3, 2>::value == 2, "");
static_assert(Probe<Set<2>, 3, 2>::value == 0, "");
static_assert(Probe<Set<2, 0>, 3, 2>::value == 1, "");
static_assert(Probe<Set<2, 0, 1>, 3, 2>::value == -1, "");

template<typename Set, int capacity, int key>
struct FindSlotHelper {
    enum {
        slot_ = KeyHash<key>::value % capacity,
        value = Probe<Set, capacity, slot_>::value
    };
};

template<typename Set, int capacity, int target, int... keys>
struct FindSlot;

template<typename Set, int capacity, int target, int key, int... keys>
struct FindSlot<Set, capacity, target, key, keys...> {
    enum {
        value = (key == target) ?
            FindSlotHelper<Set, capacity, key>::value :
            FindSlot<Set::Insert<key>::type, capacity, key, keys...>::value
    };
};

template<typename Set, int capacity, int target, int key>
struct FindSlot<Set, capacity, target, key> {
    enum {
        value = (key == target) ?
            FindSlotHelper<Set, capacity, key>::value :
            -1
    };
};

template<typename Value_>
struct Row {
    using Key   = int;
    using Value = Value_;

    Key key;
    Value value;
};

template<typename Value, int... keys>
class Table {
public:
    Table() {
    }

    template<int key>
    void insert(Value value) {
    }

    template<int key>
    const Value *find() const {
        return nullptr;
    }

private:
    enum {
        capacity = 2 * sizeof...(keys)
    };

    Row<Value> rows_[capacity];
};

int main(int argc, char **argv) {
    struct Field {
        uint32_t offset;
        uint32_t length;

        Field()
            : offset(0)
            , length(0)
        {
        }
    };

    Table<Field, 0, 1, 2, 3, 4, 5> table;
    // {
    //     Field field;
    //     field.offset = 3;
    //     field.length = 3;
    //     table.insert<0>(field);
    // }
    // {
    //     const Field *field = table.find<0>();
    //     assert(field);
    //     assert(field->offset == 3);
    //     assert(field->length == 3);
    // }

    // template<typename Set, int capacity, int target, int... keys>
    std::cout << FindSlot<Set<>, 8, 3, 0, 1, 2, 3>::value << std::endl;

    std::system("pause");
    return 0;
}
