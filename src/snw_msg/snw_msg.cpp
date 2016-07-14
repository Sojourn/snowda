#include "snw_msg.h"

using namespace Snowda;

Schema::Schema()
    : fixedSize_(0)
    , hash_(0)
{
}

void Schema::addField(uint16_t tag, uint16_t len) {
    assert(len > 0);
    assert((fixedSize_ + len) < (1 << 16));
    assert(fields_.size() < (1 << 16));

    FieldDescriptor field;
    field.tag = tag;
    field.len = len;

    fields_.push_back(field);
    fixedSize_ += len;
    hash_ ^= static_cast<uint16_t>(field.tag + (0x9e3779b9 & ((1 << 16) - 1)) + (hash_ << 6) + (hash_ >> 2));
    hash_ ^= static_cast<uint16_t>(field.len + (0x9e3779b9 & ((1 << 16) - 1)) + (hash_ << 6) + (hash_ >> 2));
}

void Schema::clear() {
    fields_.clear();
    fixedSize_ = 0;
    hash_ = 0;
}

uint16_t Schema::hash() const {
    return hash_;
}

uint16_t Schema::fieldCount() const {
    return static_cast<uint16_t>(fields_.size());
}

uint16_t Schema::fixedSize() const {
    return fixedSize_;
}

FieldDescriptor Schema::field(uint16_t index) const {
    assert(index < fields_.size());
    return fields_[index];
}

int Schema::compare(const Schema &rhs) const {
    const int lhsHash = hash_;
    const int rhsHash = rhs.hash_;
    if (lhsHash != rhsHash) {
        return lhsHash - rhsHash;
    }
    else if (fields_.size() == rhs.fields_.size()) {
        return memcmp(fields_.data(), rhs.fields_.data(), sizeof(FieldDescriptor) * fields_.size());
    }
    else {
        return fields_.size() < rhs.fields_.size() ? -1 : 1;
    }
}

FieldIndex::FieldIndex(const Schema &schema) {
    uint16_t pos = 0;
    for (uint16_t i = 0; i < schema.fieldCount(); ++i) {
        FieldDescriptor field = schema.field(i);
        offsets_.insert(std::make_pair(field.tag, pos));
        pos += field.len;
    }
}

uint16_t FieldIndex::offset(uint16_t tag) const {
    return offsets_.at(tag);
}

Message::Message(const Schema &schema, const uint8_t *data, uint16_t size)
    : schema_(schema)
    , fixedData_(data)
    , dynamicData_(data + schema.fixedSize())
    , dynamicSize_(size - schema.fixedSize())
{
    assert(schema_.fixedSize() <= size);
}

Message::Message(const Schema &schema,
    const uint8_t *fixedData, uint16_t fixedSize,
    const uint8_t *dynamicData, uint16_t dynamicSize)
    : schema_(schema)
    , fixedData_(fixedData)
    , fixedSize_(fixedSize)
    , dynamicData_(dynamicData)
    , dynamicSize_(dynamicSize)
{
}

const Schema &Message::schema() const {
    return schema_;
}

const uint8_t *Message::fixedData() const {
    return fixedData_;
}

uint16_t Message::fixedSize() const {
    return fixedSize_;
}

const uint8_t *Message::dynamicData() const {
    return dynamicData_;
}

uint16_t Message::dynamicSize() const {
    return dynamicSize_;
}

MessageBuilder::MessageBuilder()
{
    fixed_.reserve(4096);
    dynamic_.reserve(4096);
}

void MessageBuilder::clear() {
    schema_.clear();
    fixed_.clear();
    dynamic_.clear();
}

void MessageBuilder::addField(uint16_t tag, const uint8_t *data, uint16_t dataSize) {
    assert((dynamic_.size() + dataSize) < (1 << 16));

    const uint16_t pos = static_cast<uint16_t>(dynamic_.size());
    dynamic_.resize(pos + dataSize);
    memcpy(dynamic_.data() + pos, data, dataSize);
    addField(tag, pos);
}

Message MessageBuilder::message() const {
    return Message(schema_,
        fixed_.data(), static_cast<uint16_t>(fixed_.size()),
        dynamic_.data(), static_cast<uint16_t>(dynamic_.size()));
}

void testSchema() {
    Schema lhs;
    Schema rhs;
    for (size_t i = 0; i < (1 << 20); ++i) {
        lhs.clear();
        rhs.clear();

        for (size_t j = 0; j < 12; ++j) {
            if (rand() & 1) {
                lhs.addField((uint16_t)j, 1);
            }
        }

        for (size_t j = 0; j < 12; ++j) {
            if (rand() & 1) {
                rhs.addField((uint16_t)j, 1);
            }
        }

        if (lhs.compare(rhs) < 0) {
            assert(rhs.compare(lhs) > 0);
        }
        else if (lhs.compare(rhs) > 0) {
            assert(rhs.compare(lhs) < 0);
        }
        else if (lhs.compare(rhs) == 0) {
            assert(rhs.compare(lhs) == 0);

            assert(lhs.hash() == rhs.hash());
            assert(lhs.fieldCount() == rhs.fieldCount());
            assert(lhs.fixedSize() == rhs.fixedSize());

            for (size_t j = 0; j < lhs.fieldCount(); ++j) {
                FieldDescriptor lhsField = lhs.field((uint16_t)j);
                FieldDescriptor rhsField = rhs.field((uint16_t)j);
                assert(lhsField.tag == rhsField.tag);
                assert(lhsField.len == rhsField.len);
            }

            std::cout << lhs.hash() << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    MessageBuilder builder;
    builder.addField(0, 1);
    builder.addField(1, 2);
    builder.addField(2, 3);
    builder.addField(3, (const uint8_t *)"hello", 6);

    Schema schema;
    std::vector<uint8_t> buffer;
    {
        Message message = builder.message();
        schema = message.schema();
        buffer.insert(buffer.end(), message.fixedData(), message.fixedData() + message.fixedSize());
        buffer.insert(buffer.end(), message.dynamicData(), message.dynamicData() + message.dynamicSize());
    }
    {
        Message message(schema, buffer.data(), static_cast<uint16_t>(buffer.size()));
        FieldIndex index(schema);

        int a;
        int b;
        int c;
        uint16_t off;

        memcpy(&a, buffer.data() + index.offset(0), sizeof(int));
        memcpy(&b, buffer.data() + index.offset(1), sizeof(int));
        memcpy(&c, buffer.data() + index.offset(2), sizeof(int));
        memcpy(&off, buffer.data() + index.offset(3), sizeof(uint16_t));

        assert(a == 1);
        assert(b == 2);
        assert(c == 3);
        assert(off == 0);

        std::cout << (const char *)(message.dynamicData()+off) << std::endl;
    }

    std::system("pause");
    return 0;
}
