#ifndef STACK_HPP
#define STACK_HPP

#include "Vector.hpp"

template<class T>
class Stack {
private:
    Vector<T> container_;

public:
    Stack() = default;

    Stack(const Stack& other) : container_(other.container_) {}

    Stack(Stack&& other) noexcept : container_(std::move(other.container_)) {}

    Stack& operator=(const Stack& other) {
        container_ = other.container_;
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        container_ = std::move(other.container_);
        return *this;
    }

    void push(const T& value) {
        container_.push_back(value);
    }

    void push(T&& value) {
        container_.push_back(std::move(value));
    }

    void pop() {
        if (!empty()) {
            container_.pop_back();
        }
    }

    T& top() {
        return container_.back();
    }

    const T& top() const {
        return container_.back();
    }

    bool empty() const {
        return container_.empty();
    }

    size_t size() const {
        return container_.size();
    }
};

#endif // STACK_HPP