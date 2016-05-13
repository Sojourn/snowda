#ifndef SNW_PAGE_SET_H
#define SNW_PAGE_SET_H

namespace Snowda {

    class PageSet {
    public:
        PageSet();

        bool empty() const;
        bool test(size_t index) const;
        void set(size_t index);
        void clear(size_t index);

        size_t first() const;

        PageSet operator |(const PageSet &rhs) const;

    private:
        uint64_t index_[2];
    };

}

#endif // SNW_PAGE_SET_H
