#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <utility>

template<class T>
class Vector {
private:
    T* data_;
    size_t size_;
    size_t capacity_;
    static const size_t INITIAL_CAPACITY = 8;

    void reallocate(size_t new_capacity) {
        if (new_capacity == 0) {
            clear();
            ::operator delete(data_);
            data_ = nullptr;
            capacity_ = 0;
            return;
        }

        T* new_data = static_cast<T*>(::operator new(new_capacity * sizeof(T)));
        for (size_t i = 0; i < size_; ++i) {
            new(new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        ::operator delete(data_);
        data_ = new_data;
        capacity_ = new_capacity;
    }

    void ensure_capacity() {
        if (size_ >= capacity_) {
            size_t new_cap = (capacity_ == 0) ? INITIAL_CAPACITY : capacity_ * 2;
            reallocate(new_cap);
        }
    }

public:
    Vector() : data_(nullptr), size_(0), capacity_(0) {}

    void push_back(const T& value) {
        ensure_capacity();
        new(data_ + size_) T(value);
        ++size_;
    }

    void push_back(T&& value) {
        ensure_capacity();
        new(data_ + size_) T(std::move(value));
        ++size_;
    }

    void pop_back() {
        if (size_ > 0) {
            --size_;
            data_[size_].~T();
        }
    }

    T& back() {
        return data_[size_ - 1];
    }

    size_t size() const { return size_; }
    size_t capacity() const { return capacity_; }

    bool empty() const { return size_ == 0; }

    void clear() {
        for (size_t i = 0; i < size_; ++i) {
            data_[i].~T();
        }
        size_ = 0;
    }

    void reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            reallocate(new_capacity);
        }
    }

    T* begin() { return data_; }
    T* end() { return data_ + size_; }

    ~Vector() {
        clear();
        ::operator delete(data_);
    }
};

#endif