#ifndef SNW_REGION_H
#define SNW_REGION_H

namespace Snowda {

    enum class RegionProtection {
        None,
        Read,
        ReadWrite,
        ReadExecute,
    };

    class Region {
    public:
        Region();
		Region(size_t size);
        Region(const Region &) = delete;
        Region(Region &&);
		~Region();

        Region &operator=(const Region &) = delete;
        Region &operator=(Region &&);

        void modify(RegionProtection protection);
        void modify(size_t offset, size_t size, RegionProtection protection);

		uint8_t *data();
        const uint8_t *data() const;
        size_t size() const;

        explicit operator bool() const;

    private:
        uint8_t *base_;
        size_t size_;
    };

}

#endif // SNW_REGION_H
