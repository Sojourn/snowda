#include<iostream>

#include <cassert>
#include <cstdint>
#include <cstddef>

class IntrusiveListHook {
    template<typename T, IntrusiveListHook T::*hook>
    friend class IntrusiveList;
public:
    IntrusiveListHook();
    IntrusiveListHook(const IntrusiveListHook &) = delete;
    IntrusiveListHook(IntrusiveListHook &&) = delete;
    ~IntrusiveListHook();

    IntrusiveListHook &operator=(const IntrusiveListHook &) = delete;
    IntrusiveListHook &operator=(IntrusiveListHook &&) = delete;

    bool isLinked() const;
    void unlink();

private:
    IntrusiveListHook *next_;
    IntrusiveListHook *prev_;
};

template<typename T, IntrusiveListHook T::*hook>
class IntrusiveList {
public:
    template<typename T_>
    class Iterator {
    public:
        Iterator();

        Iterator operator+();
        Iterator operator+(int);

        T_ &operator*();
        T_ &operator*() const;
        T_ &operator->();
        T_ &operator->() const;

    private:
        Iterator(IntrusiveList *list, IntrusiveListHook *hook);

    private:
        IntrusiveList     *list_;
        IntrusiveListHook *hook_;
    };

public:
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;

    IntrusiveList();
    IntrusiveList(const IntrusiveList &) = delete;
    IntrusiveList(IntrusiveList &&) = delete;
    ~IntrusiveList();

    IntrusiveList &operator=(const IntrusiveList &) = delete;
    IntrusiveList &operator=(IntrusiveList &&) = delete;

    bool empty() const;

    iterator begin();
    const_iterator begin() const;
    iterator end();
    const_iterator end() const;

    void push_front(T &t);
    void push_back(T &t);
    void pop_front();
    void pop_back();
    void erase(T &t);
    void clear();

private:
    static size_t hookOffset();
    
    T &translate(IntrusiveListHook &hook);
    const T &translate(const IntrusiveListHook &hook) const;
    IntrusiveListHook &translate(T &t);
    const IntrusiveListHook &translate(const T &t) const;

private:
    IntrusiveListHook sentinel_;
};

IntrusiveListHook::IntrusiveListHook()
    : next_(nullptr)
    , prev_(nullptr)
{
}

IntrusiveListHook::~IntrusiveListHook() {
    if (isLinked()) {
        unlink();
    }
}

bool IntrusiveListHook::isLinked() const {
    return next_ != nullptr;
}

void IntrusiveListHook::unlink() {
    assert(isLinked());

    next_->prev_ = prev_;
    prev_->next_ = next_;

    next_ = nullptr;
    prev_ = nullptr;
}

template<typename T, IntrusiveListHook T::*hook>
IntrusiveList<T, hook>::IntrusiveList() {
    sentinel_.next_ = &sentinel_;
    sentinel_.prev_ = &sentinel_;
}

template<typename T, IntrusiveListHook T::*hook>
IntrusiveList<T, hook>::~IntrusiveList() {
    clear();
    sentinel_.next_ = nullptr;
    sentinel_.prev_ = nullptr;
}

template<typename T, IntrusiveListHook T::*hook>
bool IntrusiveList<T, hook>::empty() const {
    return (sentinel_.next_ == &sentinel_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::begin() -> iterator {
    return iterator(this, sentinel_.next_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::begin() const -> const_iterator {
    return const_iterator(this, sentinel_.next_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::end() -> iterator {
    return iterator(this, &sentinel_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::end() const -> const_iterator {
    return const_iterator(this, &sentinel_);
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::push_front(T &t) {
    IntrusiveListHook &hook_ = translate(t);
    assert(!hook_.isLinked());

    hook_.next_ = sentinel_.next_;
    hook_.prev_ = &sentinel_;

    sentinel_.next_->prev_ = &hook_;
    sentinel_.next_ = &hook_;
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::push_back(T &t) {
    IntrusiveListHook &hook_ = translate(t);
    assert(!hook_.isLinked());

    hook_.next_ = &sentinel_;
    hook_.prev_ = sentinel_.prev_;

    sentinel_.prev_->next_ = &hook_;
    sentinel_.prev_ = &hook_;
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::pop_front() {
    assert(!empty());

    sentinel_.next_->unlink();
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::pop_back() {
    assert(!empty());

    sentinel_.prev_->unlink();
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::erase(T &t) {
    assert(!empty());

    translate(t).unlink();
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::clear() {
    while (!empty()) {
        sentinel_.next_->unlink();
    }
}

template<typename T, IntrusiveListHook T::*hook>
size_t IntrusiveList<T, hook>::hookOffset() {
    return reinterpret_cast<uintptr_t>(&(reinterpret_cast<const T *>(nullptr)->*hook));
}

template<typename T, IntrusiveListHook T::*hook>
T &IntrusiveList<T, hook>::translate(IntrusiveListHook &hook) {
    const uintptr_t hookAddr = reinterpret_cast<uintptr_t>(&hook);
    return *reinterpret_cast<T *>(hookAddr - hookOffset());
}

template<typename T, IntrusiveListHook T::*hook>
const T &IntrusiveList<T, hook>::translate(const IntrusiveListHook &hook) const {
    const uintptr_t hookAddr = reinterpret_cast<uintptr_t>(&hook);
    return *reinterpret_cast<const T *>(hookAddr - hookOffset());
}

template<typename T, IntrusiveListHook T::*hook>
IntrusiveListHook &IntrusiveList<T, hook>::translate(T &t) {
    const uintptr_t tAddr = reinterpret_cast<uintptr_t>(&t);
    return *reinterpret_cast<IntrusiveListHook *>(tAddr + hookOffset());
}

template<typename T, IntrusiveListHook T::*hook>
const IntrusiveListHook &IntrusiveList<T, hook>::translate(const T &t) const {
    const uintptr_t tAddr = reinterpret_cast<uintptr_t>(&t);
    return *reinterpret_cast<const IntrusiveListHook *>(tAddr + hookOffset());
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
IntrusiveList<T, hook>::Iterator<T_>::Iterator()
    : list_(nullptr)
    , hook_(nullptr)
{
}

Iterator operator+();
Iterator operator+(int);

T_ &operator*();
T_ &operator*() const;
T_ &operator->();
T_ &operator->() const;

Iterator(IntrusiveList *list, IntrusiveListHook *hook);

struct Node {
    IntrusiveListHook hook1;
    IntrusiveListHook hook2;
    int value;
};

int main(int argc, char **argv) {
    IntrusiveList<Node, &Node::hook1> list;
    IntrusiveList<Node, &Node::hook1> clist;

    list.begin();
    clist.begin();
    list.end();
    clist.end();

    return 0;
}
