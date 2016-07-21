#ifndef SNW_MSG_H
#define SNW_MSG_H

#include <tuple>
#include <array>
#include <memory>
#include <vector>
#include <algorithm>

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cassert>

using FieldTag = uint16_t;

struct Slice {
    uint16_t off;
    uint16_t len;
};

#define FIELD_TYPES(X) \
    X(Int32,  int32_t) \
    X(UInt32, uint32_t) \
    X(Float,  float) \
    X(Double, double) \
    X(Str,    Slice) \
    X(Buf,    Slice)

enum class FieldType : uint16_t {
#define X(xName, xType) e##xName,
    FIELD_TYPES(X)
#undef X
    Count
};

size_t fieldTypeSize(FieldType type);
const char *fieldTypeName(FieldType type);

struct FieldDescriptor {
    FieldTag  tag;
    FieldType type;
};

struct Schema {
    const Schema          *prototype;
    uint32_t               hash;
    const FieldDescriptor *fields;
    uint16_t               fieldCount;
    uint16_t               fixedSize;
};

struct Message {
    const Message *prototype;
    const Schema  *schema;
    const uint8_t *fixedBuffer;
    uint16_t       fixedSize;
    const uint8_t *dynamicBuffer;
    uint16_t       dynamicSize;
};

class MessageBuilder {
public:
    MessageBuilder();

    void prepare(const Message *prototype = nullptr);

    void addField(FieldTag tag, int32_t val);
    void addField(FieldTag tag, const char *str);
    void addField(FieldTag tag, const char *str, size_t len);
    // TODO: Other addField methods

    Message build();

private:
    const Message               *prototype_;
    Schema                       schema_;
    std::vector<FieldDescriptor> fields_;
    std::vector<uint8_t>         fixedBuffer_;
    std::vector<uint8_t>         dynamicBuffer_;
    uint16_t                     dynamicBase_;
};

// A writer should insert the message schema into this to figure
// out if a schema message should be sent or the fixed hash
class SchemaWriterMap {
public:
    SchemaWriterMap();

    std::tuple<uint32_t, bool> insert(const Schema *schema);

private:
    void insert(std::vector<Schema>::iterator it, const Schema *schema, uint32_t hash);

    static bool compare(const Schema &lhs, const Schema &rhs);
    static bool compareFields(const Schema &lhs, const Schema &rhs);
    static std::tuple<FieldDescriptor, bool> prev(const Schema **schema, int *index);

private:
    std::vector<Schema> schemas_;
    std::vector<std::unique_ptr<FieldDescriptor[]>> fields_;
};

#endif // SNW_MSG_H
