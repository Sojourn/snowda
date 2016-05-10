#ifndef SNW_RESULT_H
#define SNW_RESULT_H

namespace Snowda {

    template<typename Value, typename Error>
    class Result
    {
    public:
        Result(Value value)
            : type_(Type::ValueType)
        {
            new(data_.value_) Value(std::move(value)); 
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
        
        Result &operator=(Result &&rhs)
        {
            if (this != &rhs) {
                this->~Result();
                type_ = rhs.type_;
                switch (type_) {
                case Type::ValueType:
                    new (&value()) Value(std::move(rhs.value()));
                    break;
                case Type::ErrorType:
                    new (&error()) Error(std::move(rhs.error()));
                    break;
                }
            }

            return *this;
        }

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

        bool hasValue() const
        {
            return type_ == Type::ValueType;
        }

        bool hasError() const
        {
            return type_ == Type::ErrorType;
        }

    private:
        enum class Type {
            ValueType,
            ErrorType,
        } type_;

        union {
#ifdef SNW_OS_WIN32
			__declspec(align(8)) uint8_t value_[sizeof(Value)];
			__declspec(align(8)) uint8_t error_[sizeof(Error)];
#else
			alignas(alignof(Value)) uint8_t value_[sizeof(Value)];
			alignas(alignof(Error)) uint8_t error_[sizeof(Error)];
#endif
        } data_;
    };

}

#endif // SNW_RESULT_H
