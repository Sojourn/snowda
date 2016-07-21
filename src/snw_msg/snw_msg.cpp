#include "snw_msg.h"

size_t fieldTypeSize(FieldType type) {
    static const size_t table[] = {
#define X(xName, xType) sizeof(xType),
    FIELD_TYPES(X)
#undef X
    };

    const size_t index = static_cast<size_t>(type);
    assert(index < static_cast<size_t>(FieldType::Count));
    return table[index];
}

const char *fieldTypeName(FieldType type) {
    static const char *table[] = {
#define X(xName, xType) #xName,
    FIELD_TYPES(X)
#undef X
    };

    const size_t index = static_cast<size_t>(type);
    assert(index < static_cast<size_t>(FieldType::Count));
    return table[index];
}

MessageBuilder::MessageBuilder()
    : prototype_(nullptr)
    , dynamicBase_(0)
{
    memset(&schema_, 0, sizeof(schema_));
    fields_.reserve(128);
    fixedBuffer_.reserve(4096);
    dynamicBuffer_.reserve(4096);
}

void MessageBuilder::prepare(const Message *prototype) {
    prototype_ = prototype;
    dynamicBase_ = 0;

    // Calculate where dynamic offsets should start
    while (prototype) {
        dynamicBase_ += prototype->dynamicSize;
        prototype = prototype->prototype;
    }

    fields_.clear();
    fixedBuffer_.clear();
    dynamicBuffer_.clear();
}

void MessageBuilder::addField(FieldTag tag, int32_t val) {
    FieldDescriptor field;
    field.tag = tag;
    field.type = FieldType::eInt32;
    fields_.push_back(field);

    const size_t pos = fixedBuffer_.size();
    fixedBuffer_.resize(pos + sizeof(val));
    memcpy(fixedBuffer_.data() + pos, &val, sizeof(val));
}

void MessageBuilder::addField(FieldTag tag, const char *str) {
    addField(tag, str, strlen(str) + 1);
}

void MessageBuilder::addField(FieldTag tag, const char *str, size_t len) {
    FieldDescriptor field;
    field.tag = tag;
    field.type = FieldType::eInt32;
    fields_.push_back(field);

    Slice val;
    val.off = static_cast<uint16_t>(dynamicBase_ + dynamicBuffer_.size());
    val.len = static_cast<uint16_t>(len);

    const size_t fixedPos = fixedBuffer_.size();
    fixedBuffer_.resize(fixedPos + sizeof(val));
    memcpy(fixedBuffer_.data() + fixedPos, &val, sizeof(val));

    const size_t dynamicPos = dynamicBuffer_.size();
    dynamicBuffer_.resize(dynamicPos + len);
    memcpy(dynamicBuffer_.data() + dynamicPos, str, len);
}

Message MessageBuilder::build() {
    if (prototype_) {
        schema_.prototype = prototype_->schema;
        schema_.hash = prototype_->schema->hash;
    }
    else {
        schema_.prototype = nullptr;
        schema_.hash = 0;
    }
    schema_.fields = fields_.data();
    schema_.fieldCount = static_cast<uint16_t>(fields_.size());
    schema_.fixedSize = 0;

    for (FieldDescriptor field: fields_) {
        schema_.fixedSize += static_cast<uint16_t>(fieldTypeSize(field.type));
        schema_.hash ^= static_cast<uint32_t>(field.tag)  + 0x9e3779b9 + (schema_.hash << 6) + (schema_.hash >> 2);
        schema_.hash ^= static_cast<uint32_t>(field.type) + 0x9e3779b9 + (schema_.hash << 6) + (schema_.hash >> 2);
    }

    Message message;
    message.prototype = prototype_;
    message.schema = &schema_;
    message.fixedBuffer = fixedBuffer_.data();
    message.fixedSize = static_cast<uint16_t>(fixedBuffer_.size());
    message.dynamicBuffer = dynamicBuffer_.data();
    message.dynamicSize = static_cast<uint16_t>(dynamicBuffer_.size());
    return message;
}

SchemaWriterMap::SchemaWriterMap() {
    schemas_.reserve(128);
    fields_.reserve(128);
}

std::tuple<uint32_t, bool> SchemaWriterMap::insert(const Schema *schema) {
    bool found = false;
    uint32_t hash = schema->hash;

    // Find a point to start the search
    auto it = std::lower_bound(schemas_.begin(), schemas_.end(), *schema, compare);

    // This should be the common case
    if ((it != schemas_.end()) && (it->hash == hash) && compareFields(*schema, *it)) {
        found = true;
    }

    // Resolve collisions
    while (!found && (it != schemas_.end()) && (it->hash == hash)) {
        if (!compareFields(*schema, *it)) {
            ++it;
            const uint32_t nextHash = hash + 1;
            if (nextHash < it->hash) {
                // Insert the schema before it
                break;
            }
            else {
                hash = nextHash;
            }
        }
        else {
            found = true;
        }
    }

    if (!found) {
        insert(it, schema, hash);
    }

    return std::make_tuple(hash, !found);
}

void SchemaWriterMap::insert(std::vector<Schema>::iterator it, const Schema *schema, uint32_t hash) {
    const Schema *tempSchema;
    uint16_t fieldCount = 0;
    uint16_t fixedSize = 0;

    tempSchema = schema;
    while (tempSchema) {
        fieldCount += tempSchema->fieldCount;
        fixedSize += tempSchema->fixedSize;
        tempSchema = tempSchema->prototype;
    }

    std::unique_ptr<FieldDescriptor[]> fields(new FieldDescriptor[fieldCount]);

    tempSchema = schema;
    size_t fieldIndex = fieldCount;
    while (tempSchema) {
        for (int i = tempSchema->fieldCount; i >= 0;) {
            fields[--fieldIndex] = tempSchema->fields[--i];
        }
    }

    Schema entry;
    entry.prototype = nullptr;
    entry.hash = hash;
    entry.fields = fields.get();
    entry.fieldCount = fieldCount;
    entry.fixedSize = fixedSize;
    schemas_.insert(it, entry);
    fields_.push_back(std::move(fields));
}

bool SchemaWriterMap::compare(const Schema &lhs, const Schema &rhs) {
    return lhs.hash < rhs.hash;
}

bool SchemaWriterMap::compareFields(const Schema &lhs, const Schema &rhs) {
    const Schema *lhsSchema = &lhs;
    const Schema *rhsSchema = &rhs;
    int lhsIndex = lhsSchema->fieldCount - 1;
    int rhsIndex = rhsSchema->fieldCount - 1;

    for (;;) {
        FieldDescriptor lhsFd;
        FieldDescriptor rhsFd;
        bool lhsFound;
        bool rhsFound;

        std::tie(lhsFd, lhsFound) = prev(&lhsSchema, &lhsIndex);
        std::tie(rhsFd, rhsFound) = prev(&rhsSchema, &rhsIndex);

        if (lhsFound && rhsFound) {
            if ((lhsFd.tag != rhsFd.tag) || (lhsFd.type != rhsFd.type)) {
                return false;
            }
        }
        else if (!lhsFound && !rhsFound) {
            return true;
        }
        else {
            return false;
        }
    }
}

std::tuple<FieldDescriptor, bool> SchemaWriterMap::prev(const Schema **schema, int *index) {
    if (*index < 0) {
        if ((*schema)->prototype) {
            *schema = (*schema)->prototype;
            *index = (*schema)->fieldCount - 1;
        }
        else {
            return std::make_tuple(FieldDescriptor(), false);
        }
    }

    return std::make_tuple((*schema)->fields[(*index)--], true);
}

int main(int argc, char **argv) {
    return 0;
}