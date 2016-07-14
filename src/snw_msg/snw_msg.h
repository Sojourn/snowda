#ifndef SNW_MSG_H
#define SNW_MSG_H

#include <vector>
#include <memory>
#include <unordered_map>
#include <iostream>

#include <cstdint>
#include <cstring>
#include <cassert>

namespace Snowda {
    struct FieldDescriptor {
        uint16_t tag;
        uint16_t len;
    };

    class Schema {
    public:
        Schema();

        void addField(uint16_t tag, uint16_t len);
        void clear();

        uint16_t hash() const;
        uint16_t fieldCount() const;
        uint16_t fixedSize() const;

        FieldDescriptor field(uint16_t index) const;
        int compare(const Schema &rhs) const;

    private:
        std::vector<FieldDescriptor> fields_;
        uint16_t fixedSize_;
        uint16_t hash_;
    };

    class FieldIndex {
    public:
        FieldIndex(const Schema &schema);

        uint16_t offset(uint16_t tag) const;

    private:
        std::unordered_map<uint16_t, uint16_t> offsets_;
    };

    class Message {
    public:
        Message(const Schema &schema, const uint8_t *data, uint16_t size);

        Message(const Schema &schema,
                const uint8_t *fixedData, uint16_t fixedSize,
                const uint8_t *dynamicData, uint16_t dynamicSize);

        const Schema &schema() const;
        const uint8_t *fixedData() const;
        uint16_t fixedSize() const;
        const uint8_t *dynamicData() const;
        uint16_t dynamicSize() const;

    private:
        const Schema &schema_;
        const uint8_t *fixedData_;
        uint16_t fixedSize_;
        const uint8_t *dynamicData_;
        uint16_t dynamicSize_;
    };

    class MessageBuilder {
    public:
        MessageBuilder();

        void clear();

        template<typename Value>
        void addField(uint16_t tag, Value value) {
            assert((fixed_.size() + sizeof(value)) < (1 << 16));

            const size_t pos = fixed_.size();
            fixed_.resize(pos + sizeof(value));
            memcpy(fixed_.data() + pos, &value, sizeof(value));

            schema_.addField(tag, sizeof(value));
        }

        void addField(uint16_t tag, const uint8_t *data, uint16_t dataSize);

        Message message() const;

    private:
        Schema schema_;
        std::vector<uint8_t> fixed_;
        std::vector<uint8_t> dynamic_;
    };
}

#endif // SNW_MSG_H
