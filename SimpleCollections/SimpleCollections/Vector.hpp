// Vector.hpp
#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>     
#include <algorithm>  
#include <memory>      

template<class T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;

    static const size_t INITIAL_CAPACITY = 8;

    void reallocate() {
        size_t new_cap = (capacity_ == 0) ? INITIAL_CAPACITY : capacity_ * 2;
        T* new_data = new T[new_cap];
        for (size_t i = 0; i < size_; ++i) {
            new_data[i] = std::move(data_[i]);
        }
        delete[] data_;
        data_ = new_data;
        capacity_ = new_cap;
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    Vector(const Vector& other) : size_(other.size_), capacity_(other.capacity_) {
        data_ = new T[capacity_];
        for (size_t i = 0; i < size_; ++i) {
            data_[i] = other.data_[i];
        }
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = other.capacity_ = 0;
    }

    ~Vector() {
        delete[] data_;
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            delete[] data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            data_ = new T[capacity_];
            for (size_t i = 0; i < size_; ++i) {
                data_[i] = other.data_[i];
            }
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            delete[] data_;
            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;
            other.data_ = nullptr;
            other.size_ = other.capacity_ = 0;
        }
        return *this;
    }

    void push_back(const T& value) {
        if (size_ >= capacity_) reallocate();
        data_[size_] = value;
        ++size_;
    }

    void push_back(T&& value) {
        if (size_ >= capacity_) reallocate();
        data_[size_] = std::move(value);
        ++size_;
    }

    size_t size() const { return size_; }

    T* begin() { return data_; }
    const T* begin() const { return data_; }

    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }
};

#endif // VECTOR_HPP