#ifndef SNW_REGION_H
#define SNW_REGION_H

namespace Snowda {

    class Region {
    public:
		Region(size_t size);
        Region(const Region &) = delete;
        Region(Region &&);
		~Region();

        Region &operator=(const Region &) = delete;
        Region &operator=(Region &&);

        void commit(size_t size);
        size_t committed() const;

		uint8_t *data();
        const uint8_t *data() const;
        size_t size() const;

        explicit operator bool() const;

    private:
        uint8_t *base_;
        size_t size_;
        size_t committed_;
    };

}

#endif // SNW_REGION_H
