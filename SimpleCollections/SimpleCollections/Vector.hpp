#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <cstddef>
#include <algorithm>
#include <memory>
#include <stdexcept>
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

        // Перемещаем существующие элементы
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

    Vector(const Vector& other) : size_(other.size_), capacity_(other.size_) {
        if (size_ > 0) {
            data_ = static_cast<T*>(::operator new(size_ * sizeof(T)));
            for (size_t i = 0; i < size_; ++i) {
                new(data_ + i) T(other.data_[i]);
            }
        }
        else {
            data_ = nullptr;
        }
    }

    Vector(Vector&& other) noexcept
        : data_(other.data_), size_(other.size_), capacity_(other.capacity_) {
        other.data_ = nullptr;
        other.size_ = 0;
        other.capacity_ = 0;
    }

    ~Vector() {
        clear();
        ::operator delete(data_);
    }

    Vector& operator=(const Vector& other) {
        if (this != &other) {
            // Создаем временную копию
            Vector temp(other);

            // Обмениваем содержимое
            std::swap(data_, temp.data_);
            std::swap(size_, temp.size_);
            std::swap(capacity_, temp.capacity_);

            // temp уничтожится с нашими старыми данными
        }
        return *this;
    }

    Vector& operator=(Vector&& other) noexcept {
        if (this != &other) {
            clear();
            ::operator delete(data_);

            data_ = other.data_;
            size_ = other.size_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.size_ = 0;
            other.capacity_ = 0;
        }
        return *this;
    }

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

    T& operator[](size_t index) {
        return data_[index];
    }

    const T& operator[](size_t index) const {
        return data_[index];
    }

    T& back() {
        if (size_ == 0) throw std::out_of_range("Vector is empty");
        return data_[size_ - 1];
    }

    const T& back() const {
        if (size_ == 0) throw std::out_of_range("Vector is empty");
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

    void shrink_to_fit() {
        if (size_ < capacity_) {
            reallocate(size_);
        }
    }

    T* begin() { return data_; }
    const T* begin() const { return data_; }

    T* end() { return data_ + size_; }
    const T* end() const { return data_ + size_; }
};

#endif // VECTOR_HPP