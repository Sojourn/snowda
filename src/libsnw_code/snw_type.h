#ifndef SNW_TYPE_H
#define SNW_TYPE_H

namespace Snowda {

    class Member {
    public:
        Member(StringView name, TypeId typeId);

        StringView name() const;
        TypeId typeId() const;

    private:
        StringView name_;
        TypeId typeId_;
    };

    class Type {
    public:
        Type(View<Function> functions, View<TypeMember> members);

        View<Function> functions() const;
        View<Member> members() const;

    private:
        View<Function> functions_;
        View<Member> members_;
    };

}

#endif // SNW_TYPE_H
