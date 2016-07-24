#include <array>
#include <list>
#include <vector>
#include <memory>
#include <iostream>

#include <iterator>
#include <cstdint>
#include <cstddef>
#include <cstdlib>
#include <cassert>

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
        friend class IntrusiveList;
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T_;
        using difference_type = std::ptrdiff_t;
        using pointer = T_ *;
        using reference = T_ &;

    public:
        Iterator();

        bool operator==(const Iterator &rhs) const;
        bool operator!=(const Iterator &rhs) const;
        Iterator operator++();
        Iterator operator++(int);
        Iterator operator--();
        Iterator operator--(int);
        reference operator*();
        reference operator*() const;
        pointer operator->();
        pointer operator->() const;

    private:
        Iterator(IntrusiveList *list, IntrusiveListHook *hook);

    private:
        IntrusiveList     *list_;
        IntrusiveListHook *hook_;
    };

public:
    using value_type = T;
    using reference = T &;
    using const_reference = const T &;
    using iterator = Iterator<T>;
    using const_iterator = Iterator<const T>;
    using difference_type = ptrdiff_t;
    using size_type = size_t;

public:
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
    iterator iterator_to(reference t);
    const_iterator iterator_to(const_reference t) const;

    reference front();
    const_reference front() const;
    reference back();
    const_reference back() const;

    void insert(iterator it, reference t);
    void push_front(reference t);
    void push_back(reference t);
    void pop_front();
    void pop_back();
    iterator erase(iterator it);
    void erase(reference t);
    void clear();

private:
    static difference_type hookOffset();
    
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
auto IntrusiveList<T, hook>::iterator_to(reference t) -> iterator {
    IntrusiveListHook &hook_ = translate(t);
    assert(hook_.isLinked());

    return iterator(this, &hook_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::iterator_to(const_reference t) const -> const_iterator {
    IntrusiveListHook &hook_ = const_cast<IntrusiveListHook &>(translate(t));
    assert(hook_.isLinked());

    return const_iterator(this, &hook_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::front() -> reference {
    assert(!empty());
    return translate(*sentinel_.next_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::front() const -> const_reference {
    assert(!empty());
    return translate(*sentinel_.next_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::back() -> reference {
    assert(!empty());
    return translate(*sentinel_.prev_);
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::back() const -> const_reference {
    assert(!empty());
    return translate(*sentinel_.prev_);
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::insert(iterator it, reference t) {
    IntrusiveListHook &nextHook = translate(*it);
    IntrusiveListHook &hook_ = translate(t);
    assert(!hook_.isLinked());

    hook_.next_ = &nextHook;
    hook_.prev_ = nextHook.prev_;

    nextHook.next_ = &hook_;
    nextHook.prev_->next_ = &hook_;
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::push_front(reference t) {
    IntrusiveListHook &hook_ = translate(t);
    assert(!hook_.isLinked());

    hook_.next_ = sentinel_.next_;
    hook_.prev_ = &sentinel_;

    sentinel_.next_->prev_ = &hook_;
    sentinel_.next_ = &hook_;
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::push_back(reference t) {
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
auto IntrusiveList<T, hook>::erase(iterator it) -> iterator {
    assert(it != end());
    translate(*it).unlink();
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::erase(reference t) {
    assert(!empty());
    translate(t).unlink();
}

template<typename T, IntrusiveListHook T::*hook>
void IntrusiveList<T, hook>::clear() {
    while (!empty()) {
        pop_back();
    }
}

template<typename T, IntrusiveListHook T::*hook>
auto IntrusiveList<T, hook>::hookOffset() -> difference_type {
    auto tAddr = reinterpret_cast<const T *>(nullptr);
    auto hookAddr = &(tAddr->*hook);
    return static_cast<difference_type>(
        reinterpret_cast<uintptr_t>(hookAddr) - reinterpret_cast<uintptr_t>(tAddr));
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

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
IntrusiveList<T, hook>::Iterator<T_>::Iterator(IntrusiveList *list, IntrusiveListHook *hook)
    : list_(list)
    , hook_(hook)
{
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
bool IntrusiveList<T, hook>::Iterator<T_>::operator==(const Iterator &rhs) const {
    return (hook_ == rhs.hook_);
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
bool IntrusiveList<T, hook>::Iterator<T_>::operator!=(const Iterator &rhs) const {
    return !this->operator==(rhs);
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator++() -> Iterator {
    hook_ = hook_->next_;
    return *this;
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator++(int) -> Iterator {
    Iterator it(*this);
    hook_ = hook_->next_;
    return it;
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator--() -> Iterator {
    hook_ = hook_->prev_;
    return *this;
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator--(int) -> Iterator {
    Iterator it(*this);
    hook_ = hook_->prev_;
    return it;
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator*() -> reference {
    return list_->translate(*hook_);
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator*() const -> reference {
    return list_->translate(*hook_);
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator->() -> pointer {
    return &list_->translate(*hook_);
}

template<typename T, IntrusiveListHook T::*hook>
template<typename T_>
auto IntrusiveList<T, hook>::Iterator<T_>::operator->() const -> pointer {
    return &list_->translate(*hook_);
}

struct Node {
    int value;
    IntrusiveListHook hook;
};

// class IntrusiveListFuzzer {
// public:
//     IntrusiveListFuzzer(size_t nodeCount)
//         : nodes_(new Node[nodeCount])
//         , nodeCount_(nodeCount)
//     {
//         freeNodes_.reserve(nodeCount);
//         for (size_t i = 0; i < nodeCount_; ++i) {
//             Node &node = nodes_[i];
// 			node.value = static_cast<int>(i);
//             freeNodes_.push_back(&node);
//         }
//     }

//     void step() {
//     }

// private:
//     void push_front() {
//         if (!freeNodes_.empty()) {
//             Node *node = freeNodes_.back();
//             freeNodes_.pop_back();
// 			list_.push_front(*node);
// 			intrusiveList_.push_front(*node);
//         }

//         compare();
//     }

//     void push_back() {
//         if (!freeNodes_.empty()) {
//             Node *node = freeNodes_.back();
//             freeNodes_.pop_back();
// 			list_.push_back(*node);
// 			intrusiveList_.push_back(*node);
//         }

//         compare();
//     }

//     void clear() {
//         list_.clear();
//         intrusiveList_.clear();

//         assert(list_.empty());
//         assert(intrusiveList_.empty());

//         freeNodes_.clear();
//         for (size_t i = 0; i < nodeCount_; ++i) {
//             Node &node = nodes_[i];
//             assert(!node.hook.isLinked());

//             freeNodes_.push_back(&node);
//         }

//         compare();
//     }

//     void pop_front() {
//         if (!list_.empty()) {
//         }

//         compare();
//     }

//     void pop_back() {
//         if (!list_.empty()) {
//         }

//         compare();
//     }

//     void compare() {
//         assert(list_.empty() == intrusiveList_.empty());

//         auto listPos = list_.begin();
//         auto listEnd = list_.end();
//         auto intrusiveListPos = intrusiveList_.begin();
//         auto intrusiveListEnd = intrusiveList_.end();

//         assert(std::distance(listPos, listEnd) == std::distance(intrusiveListPos, intrusiveListEnd));
//         for (;;) {
//             if (listPos == listEnd) {
//                 assert(intrusiveListPos == intrusiveListEnd);
//                 break;
//             }
//             else {
//                 assert(listPos->value == intrusiveListPos->value);
//             }
//         }
//     }

// private:
//     const std::unique_ptr<Node[]> nodes_;
//     const size_t nodeCount_;
//     std::vector<Node *> freeNodes_;
//     std::list<Node>   list_;
//     IntrusiveList<Node, &Node::hook> intrusiveList_;
// };

int main(int argc, char **argv) {
    IntrusiveList<Node, &Node::hook> list;
    Node n1; n1.value = 1;
    Node n2; n2.value = 2;
    Node n3; n3.value = 3;
    Node n4; n4.value = 4;

    list.push_front(n1);
    list.push_back(n2);
    list.push_back(n4);

    list.erase(n1);

    for (const Node &node: list) {
        std::cout << node.value << std::endl;
    }

    std::system("pause");
    return 0;
}
