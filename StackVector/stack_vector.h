#pragma once

#include <array>
#include <stdexcept>

template <typename T, size_t kCapacity>
class StackVector {
public:
    explicit StackVector(size_t a_size = 0) {
        if (a_size > kCapacity) {
            throw std::invalid_argument("size cannot exceed capacity");
        }
        
        size_ = a_size;
    }

    T& operator[](size_t index) {
        return container_[index];
    }
    
    const T& operator[](size_t index) const {
        return container_[index];
    }

    auto begin() {
        return container_.begin();
    }
    
    auto end() {
        return begin() + size_;
    }
    
    const auto begin() const {
        return container_.begin();
    }
    
    const auto end() const {
        return begin() + size_;
    }

    size_t Size() const {
        return size_;
    }
    
    size_t Capacity() const {
        return kCapacity;
    }

    void PushBack(const T& value) {
        if (size_ < kCapacity) {
            container_[size_] = value;
            ++size_;
            return;
        }
        
        throw std::overflow_error("no capacity left to add another element");
    }
    
    T PopBack() {
        if (size_ == 0) {
            throw std::underflow_error("StackVector is empty");
        }
        
        --size_;
        
        return container_[size_];
    }
    
private:
    std::array<T, kCapacity> container_;
    size_t size_ = 0;
};
