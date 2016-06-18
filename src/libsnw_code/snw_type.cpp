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


Type(View<Function> functions, View<TypeMember> members);

View<Function> functions() const;
View<Member> members() const;

