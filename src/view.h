#ifndef SNOWDA_VIEW_H
#define SNOWDA_VIEW_H

#include <cstring>
#include <cassert>

namespace Snowda {

    template<typename T>
    class View {
    public:
        View()
            : begin_(nullptr)
            , end_(nullptr)
        {
        }

        View(const T *first, const T *last)
            : begin_(first)
            , end_(last)
        {
            assert(begin_ <= end_);
        }

        size_t size() const
        {
            return end_ - begin_;
        }

        bool empty() const
        {
            return size() == 0;
        }

        const T *begin() const
        {
            return begin_;
        }

        const T *end() const
        {
            return end_;
        }

        const T &front() const
        {
            assert(!empty());
            return *begin_;
        }

        const T &back() const
        {
            assert(!empty());
            return *(end_ - 1);
        }

        const T &operator[](size_t index) const
        {
            return begin_[index];
        }

        View<T> slice(size_t first, size_t last) const
        {
            assert((first <= last) && (last <= size()));
            return View<T>(begin_ + first, begin_ + last);
        }

        View<T> head(size_t last) const
        {
            return slice(0, last);
        }

        View<T> tail(size_t first) const
        {
            return slice(first, size());
        }

    private:
        const T *begin_;
        const T *end_;
    };

    template<typename T>
    static std::ostream &operator<<(std::ostream &os, const View<T> &view)
    {
        for (auto &value: view) {
            os << value;
        }

        return os;
    }

    class StringView : public View<char> {
    public:
        template<char c>
        static StringView chr()
        {
            static const char str[] = { c };
            return StringView(str, str + sizeof(str));
        }

        StringView()
        {
        }

        StringView(View<char> view)
            : View<char>(view)
        {
        }

        StringView(const char *str)
            : View<char>(str, str + strlen(str))
        {
        }

        StringView(const char *first, const char *last)
            : View<char>(first, last)
        {
        }

        StringView &operator=(const char *str)
        {
            return operator=(StringView(str));
        }

        bool operator==(StringView rhs) const
        {
            if (size() != rhs.size()) {
                return false;
            }
            else {
                return memcmp(begin(), rhs.begin(), size()) == 0;
            }
        }

        bool operator==(const char *str) const
        {
            return operator ==(StringView(str));
        }

        bool startsWith(StringView other)
        {
            if (size() < other.size()) {
                return false;
            }
            else {
                for (size_t i = 0; i < other.size(); ++i) {
                    if ((*this)[i] != other[i]) {
                        return false;
                    }
                }

                return true;
            }
        }
    };

}

#endif // SNOWDA_VIEW_H
