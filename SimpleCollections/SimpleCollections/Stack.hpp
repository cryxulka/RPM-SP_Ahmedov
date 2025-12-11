// Stack.hpp
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
        if (this != &other) {
            container_ = other.container_;
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            container_ = std::move(other.container_);
        }
        return *this;
    }

    void push(const T& value) { container_.push_back(value); }
    void push(T&& value) { container_.push_back(std::move(value)); }

    void pop() {
        if (!empty()) {
            container_.size();
        }
    }

    const T& top() const {
        return *(container_.end() - 1);
    }

    bool empty() const {
        return container_.size() == 0;
    }
};

#endif // STACK_HPP