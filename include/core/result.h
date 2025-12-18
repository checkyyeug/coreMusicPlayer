#ifndef CORE_RESULT_H
#define CORE_RESULT_H

#include <stdexcept>
#include <string>

namespace core {

template<typename T>
class Result {
public:
    // 构造成功值
    explicit Result(const T& value) : is_error_(false) {
        new(&data_.value) T(value);
    }

    // 构造成功值（移动）
    explicit Result(T&& value) : is_error_(false) {
        new(&data_.value) T(std::move(value));
    }

    // 构造错误值
    explicit Result(const std::string& error) : is_error_(true) {
        new(&data_.error) std::string(error);
    }

    // 拷贝构造函数
    Result(const Result<T>& other) : is_error_(other.is_error_) {
        if (is_error_) {
            new(&data_.error) std::string(other.data_.error);
        } else {
            new(&data_.value) T(other.data_.value);
        }
    }

    // 移动构造函数
    Result(Result<T>&& other) noexcept : is_error_(other.is_error_) {
        if (is_error_) {
            new(&data_.error) std::string(std::move(other.data_.error));
        } else {
            new(&data_.value) T(std::move(other.data_.value));
        }
    }

    // 赋值操作符（拷贝）
    Result<T>& operator=(const Result<T>& other) {
        if (this != &other) {
            cleanup();
            is_error_ = other.is_error_;
            if (is_error_) {
                new(&data_.error) std::string(other.data_.error);
            } else {
                new(&data_.value) T(other.data_.value);
            }
        }
        return *this;
    }

    // 赋值操作符（移动）
    Result<T>& operator=(Result<T>&& other) noexcept {
        if (this != &other) {
            cleanup();
            is_error_ = other.is_error_;
            if (is_error_) {
                new(&data_.error) std::string(std::move(other.data_.error));
            } else {
                new(&data_.value) T(std::move(other.data_.value));
            }
        }
        return *this;
    }

    // 析构函数
    ~Result() {
        cleanup();
    }

    // 检查是否为成功状态
    bool is_ok() const { return !is_error_; }

    // 检查是否为错误状态
    bool is_err() const { return is_error_; }

    // 获取值（如果存在）
    T& value() {
        if (is_error_) {
            throw std::runtime_error("Trying to access value of an error Result");
        }
        return data_.value;
    }

    // 获取错误信息
    const std::string& error() const {
        if (!is_error_) {
            throw std::runtime_error("Trying to access error of a successful Result");
        }
        return data_.error;
    }

private:
    void cleanup() {
        if (is_error_) {
            data_.error.~std::string();
        } else {
            data_.value.~T();
        }
    }

    union {
        T value;
        std::string error;
    } data_;
    bool is_error_;
};

// 成功结果的便捷函数
template<typename T>
Result<T> Ok(const T& value) {
    return Result<T>(value);
}

// 错误结果的便捷函数
template<typename T>
Result<T> Err(const std::string& error) {
    return Result<T>(error);
}

} // namespace core

#endif // CORE_RESULT_H