#ifndef SNW_FIXED_VECTOR_H
#define SNW_FIXED_VECTOR_H

namespace Snowda {

    template<typename T, size_t capacity_, typename size_type_ = size_t>
    class FixedVector {
    public:
        static_assert(capacity_ > 0, "Must have a non-zero capacity");
        static_assert(std::numeric_limits<size_type_>::max() >= capacity_, "size_type insufficient");

        using size_type = size_type_;
        using iterator = T *;
        using const_iterator = const T *;

        FixedVector()
            : size_(0)
        {
        }

        // This is unsafe without dependant types
        // template<typename Iterator>
        // FixedVector(Iterator first, Iterator last);

        FixedVector(FixedVector &&) = delete;
        FixedVector(const FixedVector &) = delete;

        ~FixedVector()
        {
            clear();
        }

        FixedVector &operator=(FixedVector &&) = delete;
        FixedVector &operator=(const FixedVector &) = delete;

        iterator data()
        {
            return reinterpret_cast<T *>(buffer_);
        }

        const_iterator data() const
        {
            return reinterpret_cast<const T *>(buffer_);
        }

        iterator begin()
        {
            return data();
        }

        const_iterator begin() const
        {
            return data();
        }

        iterator end()
        {
            return data() + size();
        }

        const_iterator end() const
        {
            return data() + size();
        }

        size_type size() const
        {
            return size_;
        }

        size_t capacity() const
        {
            return capacity_;
        }

        bool empty() const
        {
            return size_ == 0;
        }

        // FIXME: Return a status or blow up the program?
        bool push_back(const T &value)
        {
            if (size() < capacity()) {
                new(&data()[size_++]) T(value);
                return true;
            }
            else {
                return false;
            }
        }

        // FIXME: Return a status or blow up the program?
        bool push_back(T &&value)
        {
            if (size() < capacity()) {
                new(&data()[size_++]) T(std::move(value));
                return true;
            }
            else {
                return false;
            }
        }

        void pop_back()
        {
            if (!empty()) {
                data()[--size_].~T();
            }
        }

        void clear()
        {
            for (auto &value: *this) {
                value.~T();
            }
            size_ = 0;
        }

        // iterator erase(iterator pos);
        // iterator erase(const_iterator pos);
        // iterator erase(iterator first, iterator last);
        // iterator erase(const_iterator first, const_iterator last);

    private:
        size_type_ size_;

		// FIXME: Alignment on windows
#ifdef SNW_OS_WIN32
		// FIXME: Variable alignment
		__declspec(align(8)) uint8_t buffer_[sizeof(T) * capacity_];
#else
        alignas(alignof(T)) uint8_t buffer_[sizeof(T) * capacity_];
#endif
    };

}

#endif // SNOWDA_FIXED_VECTOR_H
