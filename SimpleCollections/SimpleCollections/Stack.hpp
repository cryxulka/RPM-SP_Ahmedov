#ifndef STACK_HPP
#define STACK_HPP

#include <cstddef>
#include <utility>

template<class T>
class Stack {
private:
    struct Node {
        T data;
        Node* next;

        Node(const T& value, Node* nxt = nullptr) : data(value), next(nxt) {}
        Node(T&& value, Node* nxt = nullptr) : data(std::move(value)), next(nxt) {}
    };

    Node* top_;
    size_t size_;

public:
    Stack() : top_(nullptr), size_(0) {}

    Stack(const Stack& other) : top_(nullptr), size_(0) {
        if (!other.empty()) {
            Node* current = other.top_;
            Stack temp;
            while (current) {
                temp.push(current->data);
                current = current->next;
            }
            while (!temp.empty()) {
                this->push(temp.top());
                temp.pop();
            }
        }
    }

    Stack(Stack&& other) noexcept : top_(other.top_), size_(other.size_) {
        other.top_ = nullptr;
        other.size_ = 0;
    }

    Stack& operator=(const Stack& other) {
        if (this != &other) {
            clear();
            if (!other.empty()) {
                Node* current = other.top_;
                Stack temp;
                while (current) {
                    temp.push(current->data);
                    current = current->next;
                }
                while (!temp.empty()) {
                    this->push(temp.top());
                    temp.pop();
                }
            }
        }
        return *this;
    }

    Stack& operator=(Stack&& other) noexcept {
        if (this != &other) {
            clear();
            top_ = other.top_;
            size_ = other.size_;
            other.top_ = nullptr;
            other.size_ = 0;
        }
        return *this;
    }

    ~Stack() {
        clear();
    }

    void push(const T& value) {
        top_ = new Node(value, top_);
        ++size_;
    }

    void push(T&& value) {
        top_ = new Node(std::move(value), top_);
        ++size_;
    }

    void pop() {
        if (top_) {
            Node* old_top = top_;
            top_ = top_->next;
            delete old_top;
            --size_;
        }
    }

    T& top() {
        return top_->data;
    }

    const T& top() const {
        return top_->data;
    }

    bool empty() const {
        return top_ == nullptr;
    }

    size_t size() const {
        return size_;
    }

    void clear() {
        while (!empty()) {
            pop();
        }
    }
};

#endif