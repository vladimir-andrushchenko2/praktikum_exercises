#pragma once

#include <cassert>
#include <initializer_list>
#include <string>
#include <stdexcept>

#include "array_ptr.h"

using namespace std::literals;


struct ReserveProxyObject {
    explicit ReserveProxyObject(size_t capacity): capacity_to_reserve(capacity) {}
    size_t capacity_to_reserve;
};

auto Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObject(capacity_to_reserve);
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    SimpleVector() noexcept = default;
    
    explicit SimpleVector(size_t size): size_(size), capacity_(size), begin_(new Type[size]) {
        std::fill(begin(), end(), Type{});
    }
    
    SimpleVector(size_t size, const Type& value): size_(size), capacity_(size), begin_(new Type[size]) {
        std::fill(begin(), end(), value);
    }
    
    SimpleVector(std::initializer_list<Type> init) {
        Resize(init.size());
        std::copy(init.begin(), init.end(), begin_.Get());
    }
    
    SimpleVector(const SimpleVector& other) {
        Resize(other.GetSize());
        std::copy(other.begin(), other.end(), begin_.Get());
    }
    
    SimpleVector(const ReserveProxyObject& reserve_proxy) {
        Reserve(reserve_proxy.capacity_to_reserve);
    }
    
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            SimpleVector temp(other);
            swap(temp);
        }
        
        return *this;
    }
    
public:
    void PushBack(const Type& value) {
        ManageCapacity();
        
        begin_[size_] = value;
        ++size_;
    }
    
    void PopBack() noexcept {
        assert(!IsEmpty());
        Resize(GetSize() - 1);
    }
    
    Iterator Insert(ConstIterator pos, const Type& value) {
        // if empty begin == nullptr, can't insert or calculate index
        if (IsEmpty()) {
            PushBack(value);
            return begin();
        }
        
        auto index = std::distance(cbegin(), pos);
        
        ManageCapacity();
        
        std::copy_backward(begin() + index, end(), end() + 1);
        
        begin_[index] = value;
        
        ++size_;
        
        return begin() + index;
    }
    
    Iterator Erase(ConstIterator pos) {
        auto index = pos - begin();
        std::copy(begin() + index + 1, end(), begin() + index);
        PopBack();
        return begin() + index;
    }
    
    size_t GetSize() const noexcept {
        return size_;
    }
    
    size_t GetCapacity() const noexcept {
        return capacity_;
    }
    
    bool IsEmpty() const noexcept {
        return GetSize() == 0;
    }
    
    Type& operator[](size_t index) noexcept {
        return begin_[index];
    }
    
    const Type& operator[](size_t index) const noexcept {
        return begin_[index];
    }
    
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index overflow"s);
        }
        
        return begin_[index];
    }
    
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index overflow"s);
        }
        
        return begin_[index];
    }
    
    void Clear() noexcept {
        Resize(0);
    }
    
    void Resize(size_t new_size) {
        if (new_size <= capacity_) {
            if (new_size > GetSize()) {
                // increase size
                std::fill(end(), begin() + new_size, Type{});
                size_ = new_size;
                return;
            }
            
            // decrease size
            auto old_size = GetSize();
            size_ = new_size;
            std::fill(end(), begin() + old_size, Type{});
        } else {
            CopyAndSwapNElements(begin(), GetSize(), new_size);
        }
    }
    
    void Reserve(size_t new_capacity) {
        auto old_size = GetSize();
        CopyAndSwapNElements(begin(), std::min(GetSize(), new_capacity), new_capacity);
        size_ = old_size;
    }
    
public:
    Iterator begin() noexcept {
        return begin_.Get();
    }
    
    Iterator end() noexcept {
        return begin_.Get() + GetSize();
    }
    
    ConstIterator begin() const noexcept {
        return begin_.Get();
    }
    
    ConstIterator end() const noexcept {
        return begin_.Get() + GetSize();
    }
    
    ConstIterator cbegin() const noexcept {
        return begin_.Get();
    }
    
    ConstIterator cend() const noexcept {
        return begin_.Get() + GetSize();
    }
    
    void swap(SimpleVector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        begin_.swap(other.begin_);
    }
    
private:
    void DoubleOrInitializeCapacity() {
        auto old_size = GetSize();
        Resize(GetCapacity() == 0 ? 1 : GetCapacity() * 2);
        size_ = old_size;
    }
    
    void CopyAndSwapNElements(ConstIterator source, size_t n_elements, size_t new_capacity) {
        SimpleVector temp(new_capacity);
        std::copy(source, source + n_elements, temp.begin());
        swap(temp);
    }
    
    void ManageCapacity() {
        if (GetSize() == GetCapacity()) {
            DoubleOrInitializeCapacity();
        }
    }
    
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    
    ArrayPointer<Type> begin_;
};


template <typename Type>
bool operator==(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    if (&left == &right) {
        return true;
    }
    
    if (left.GetSize() != right.GetSize()) {
        return false;
    }
    
    return std::equal(left.begin(), left.end(), right.begin());
}

template <typename Type>
bool operator!=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return !(left == right);
}

template <typename Type>
bool operator<(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return std::lexicographical_compare(left.begin(), left.end(), right.begin(), right.end());
}

template <typename Type>
bool operator<=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return !(right < left);
}

template <typename Type>
bool operator>(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return right < left;
}

template <typename Type>
bool operator>=(const SimpleVector<Type>& left, const SimpleVector<Type>& right) {
    return right <= left;
}
