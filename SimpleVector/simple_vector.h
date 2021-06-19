#pragma once

#include <cassert>
#include <initializer_list>
#include <string>
#include <stdexcept>
#include <memory>

#include "array_ptr.h"

using namespace std::literals;


struct ReserveProxyObject {
    explicit ReserveProxyObject(size_t capacity): capacity_to_reserve(capacity) {}
    size_t capacity_to_reserve;
};

ReserveProxyObject Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObject(capacity_to_reserve);
}

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    SimpleVector() noexcept = default;
    
    explicit SimpleVector(size_t size): size_(size), capacity_(size), begin_(new Type[size]) {
        std::uninitialized_fill(begin(), end(), Type{});
    }
    
    SimpleVector(size_t size, const Type& value): size_(size), capacity_(size), begin_(new Type[size]) {
        std::uninitialized_fill(begin(), end(), value);
    }
    
    SimpleVector(std::initializer_list<Type> init): size_(init.size()), capacity_(init.size()), begin_(new Type[init.size()]) {
        std::uninitialized_move(init.begin(), init.end(), begin_.Get());
    }
    
    SimpleVector(const SimpleVector& other) {
        SimpleVector temp(other.GetSize());
        std::copy(other.begin(), other.end(), temp.begin());
        swap(temp);
    }
    
    SimpleVector(const ReserveProxyObject& reserve_proxy) {
        Reserve(reserve_proxy.capacity_to_reserve);
    }
    
    SimpleVector(SimpleVector&& other) {
        swap(other);
    }
    
    SimpleVector& operator=(const SimpleVector& other) {
        if (this != &other) {
            SimpleVector temp(other);
            swap(temp);
        }
        
        return *this;
    }
    
    SimpleVector& operator=(SimpleVector&& other) {
        if (this != &other) {
            SimpleVector temp;
            temp.swap(other);
            swap(temp);
        }
        
        return *this;
    }
    
public:
    void PushBack(const Type& value) {
        ManageCapacity();
        At(size_++) = value;
    }
    
    void PushBack(Type&& value) {
        ManageCapacity();
        At(size_++) = std::move(value);
    }
    
    void PopBack() noexcept {
        assert(!IsEmpty());
        Resize(GetSize() - 1);
    }
    
    Iterator Insert(ConstIterator pos, const Type& value) {
        return DoInsert(pos, value);
    }
    
    Iterator Insert(ConstIterator pos, Type&& value) {
        return DoInsert(pos, std::move(value));
    }
    
    Iterator Erase(ConstIterator pos) {
        assert(!IsEmpty());
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
    
    void Clear() noexcept {
        Resize(0);
    }
    
    void Resize(size_t new_size) {
        if (new_size <= capacity_) {
            // increase size
            if (new_size >= GetSize()) {
                std::uninitialized_fill(end(), begin() + new_size, Type{});
                size_ = new_size;
                return;
            }
            
            // decrease size
            auto old_end = begin() + GetSize();
            size_ = new_size;
            std::fill(end(), old_end, Type{});
        } else {
            Reallocate(begin(), end(), new_size);
            Resize(new_size);
        }
    }
    
    void Reserve(size_t new_capacity) {
        // new capacity cannot be less
        if (new_capacity <= GetCapacity()) {
            return;
        }
        
        Reallocate(begin(), end(), new_capacity);
    }
    
public:
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
    
    void swap(SimpleVector& other) noexcept {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        begin_.swap(other.begin_);
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
    
private:
    void Reallocate(Iterator begin, Iterator end, size_t new_capacity) {
        assert((end - begin) >= 0);
        assert(static_cast<size_t>(end - begin) <= new_capacity);
        
        ArrayPointer<Type> new_memory(new_capacity);
        std::uninitialized_move(begin, end, new_memory.Get());
        
        begin_.swap(new_memory);
        capacity_ = new_capacity;
    }
    
    void ManageCapacity() {
        if (GetSize() == GetCapacity()) {
            DoubleOrInitializeCapacity();
        }
    }
    
    void DoubleOrInitializeCapacity() {
        Reserve(GetCapacity() == 0 ? 1 : GetCapacity() * 2);
    }
    
    
    Iterator DoInsert(ConstIterator pos, Type value) {
        // if empty => begin == nullptr, so can't insert or calculate index
        if (IsEmpty()) {
            PushBack(std::move(value));
            return begin();
        }
        
        auto index = pos - cbegin();
        
        ManageCapacity();
        
        std::copy_backward(std::make_move_iterator(begin() + index), std::make_move_iterator(end()), end() + 1);
        
        ++size_;
        
        At(index) = std::move(value);
        
        return begin() + index;
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
