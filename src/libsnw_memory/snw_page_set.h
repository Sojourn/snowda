#ifndef SNW_PAGE_SET_H
#define SNW_PAGE_SET_H

namespace Snowda {

    class PageSet {
    public:
        PageSet();

        bool empty() const;
        bool test(uint8_t pos) const;
        void set(uint8_t pos);
        void clear(uint8_t pos);

        std::tuple<uint8_t, bool> firstSet() const;
        std::tuple<uint8_t, bool> firstCleared() const;

    private:
        std::array<uint64_t, 4> index_;
    };

}

#endif // SNW_PAGE_SET_H
