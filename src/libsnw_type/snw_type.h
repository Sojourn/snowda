#ifndef SNW_TYPE_H
#define SNW_TYPE_H

#include <map>
#include <tuple>

#include <cstdint>
#include <cassert>

#include "snw_util.h"

namespace Snowda {

    struct Type;
    struct TypeMember;
    struct TypeAttribute;

    struct TypeMember {
        StringView name;
        uint32_t offset;
        uint32_t size;
    };

    struct TypeAttribute {
        StringView name;
        StringView value;
    };

    struct Type {
        StringView name;
        size_t memberCount;
        size_t attributeCount;
        const TypeMember *members;
        const TypeAttribute *attributes;
    };

    class TypeSystem {
    public:
        void addType(const Type &type)
        {
            assert(types_.count(type.name) == 0);
            types_.insert(std::make_pair(type.name, type));
        }

        const Type *findType(StringView typeName) const
        {
            auto iter = types_.find(typeName);
            if (iter != types_.end()) {
				return &std::get<1>(*iter);
            }
            else {
                return nullptr;
            }
        }

    private:
        std::map<StringView, Type> types_;
    };

}

#endif // SNW_TYPE_H
