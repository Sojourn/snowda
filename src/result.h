#ifndef SNOWDA_RESULT_H
#define SNOWDA_RESULT_H

#include <type_traits>

#include <cstdint>
#include <cassert>

namespace Snowda {

    template<typename Value, typename Error>
    class Result
    {
    public:
        Result(Value value)
            : type_(Type::ValueType)
        {
            new(data_.error_) Value(std::move(value)); 
        }

        Result(Error error)
            : type_(Type::ErrorType)
        {
            new(data_.error_) Error(std::move(error));
        }

        Result(Result &&other)
            : type_(other.type_)
        {
            switch (type_) {
            case Type::ValueType:
                new(data_.value_) Value(std::move(other.value()));
                break;
            case Type::ErrorType:
                new(data_.error_) Error(std::move(other.error()));
                break;
            }
        }

        Result(const Result &) = delete;

        ~Result()
        {
            switch (type_) {
            case Type::ValueType:
                value().~Value();
                break;
            case Type::ErrorType:
                error().~Error();
                break;
            }
        }

        Result &operator=(const Result &) = delete;
        Result &operator=(Result &&) = delete;

        explicit operator bool() const
        {
            return type_ == Type::ValueType;
        }

        Value &value()
        {
            assert(type_ == Type::ValueType);
            return *reinterpret_cast<Value *>(data_.value_);
        }

        Error &error()
        {
            assert(type_ == Type::ErrorType);
            return *reinterpret_cast<Error *>(data_.error_);
        }

        template<typename OkFunc, typename ErrFunc>
        void match(OkFunc &&ok, ErrFunc &&err)
        {
            switch (type_) {
            case Type::ValueType:
                ok(value());
                break;
            case Type::ErrorType:
                err(error());
                break;
            }
        }

    private:
        enum class Type {
            ValueType,
            ErrorType,
        } type_;

        union {
            alignas(alignof(Value)) uint8_t value_[sizeof(Value)];
            alignas(alignof(Error)) uint8_t error_[sizeof(Error)];
        } data_;
    };

}

#endif // SNOWDA_RESULT_H
