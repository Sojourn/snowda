#include "snw_code.h"

using namespace Snowda;

Member::Member(StringView name, TypeId typeId)
    : name_(name)
    , typeId_(typeId)
{
}

StringView Member::name() const
{
    return name_;
}

TypeId Member::typeId() const
{
    return typeId_;
}


Type::Type(View<Function> functions, View<Member> members)
    : functions_(functions)
    , members_(members)
{
}

View<Function> Type::functions() const
{
    return functions_;
}

View<Member> Type::members() const
{
    return members_;
}
