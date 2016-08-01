#ifndef SNW_INTRUSIVE_LIST_H
#define SNW_INTRUSIVE_LIST_H

namespace Snowda {
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
        translate(*(it++)).unlink();
        return it;
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
}

#endif // SNW_INTRUSIVE_LIST_H
