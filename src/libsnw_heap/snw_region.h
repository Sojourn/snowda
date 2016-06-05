#ifndef SNW_REGION_H
#define SNW_REGION_H

namespace Snowda {

    class Region {
    public:
		Region();
        Region(const Region &) = delete;
        Region(Region &&) = delete;
		~Region();

        Region &operator=(const Region &) = delete;
        Region &operator=(Region &&) = delete;

		uint8_t *data();
        const uint8_t *data() const;
        size_t size() const;

        explicit operator bool() const;

    private:
        uint8_t *base_;
    };

}

#endif // SNW_REGION_H
