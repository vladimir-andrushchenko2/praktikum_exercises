#pragma once

#include <cassert>
#include <initializer_list>
#include <string>
#include <stdexcept>

#include "array_ptr.h"

using namespace std::literals;

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    
    SimpleVector() noexcept = default;
    
    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size): size_(size), capacity_(size), begin_(new Type[size]) {
        std::fill(begin(), end(), Type{});
    }
    
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value): size_(size), capacity_(size), begin_(new Type[size]) {
        std::fill(begin(), end(), value);
    }
    
    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        Resize(init.size());
        std::copy(init.begin(), init.end(), begin_.Get());
    }
    
    SimpleVector(const SimpleVector& other) {
        Resize(other.GetSize());
        std::copy(other.begin(), other.end(), begin_.Get());
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
        // если вектор пуст то его begin это nullptr, а мы туда вставить не можем и вычислить index тоже
        if (IsEmpty()) {
            PushBack(value);
            return begin();
        }
        
        auto index = std::distance(pos, cbegin());
        
        ManageCapacity();

        std::copy_backward(begin() + index, end(), end() + 1);

        begin()[index] = value;
        
        ++size_;

        return begin() + index;
    }
    
    Iterator Erase(ConstIterator pos) {
        auto index = pos - begin();
        std::copy(begin() + index + 1, end(), begin() + index);
        PopBack();
        return begin() + index;
    }
    
    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        return size_;
    }
    
    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        return capacity_;
    }
    
    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return GetSize() == 0;
    }
    
    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        return begin_[index];
    }
    
    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return begin_[index];
    }
    
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_) {
            throw std::out_of_range("index overflow"s);
        }
        
        return begin_[index];
    }
    
    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_) {
            throw std::out_of_range("index overflow"s);
        }
        
        return begin_[index];
    }
    
    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        Resize(0);
    }
    
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size <= capacity_) {
            if (new_size > GetSize()) {
                // это в случае увеличения в пределах существующей емкости;
                std::fill(end(), begin() + new_size, Type{});
                size_ = new_size;
                return;
            }
            
            // это в случае уменьшения в пределах существующей емкости;
            auto old_size = GetSize();
            size_ = new_size;
            std::fill(end(), begin() + old_size, Type{});
        } else {
            SimpleVector temp(new_size);
            std::copy(begin(), end(), temp.begin());
            swap(temp);
        }
    }
    
    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        return begin_.Get();
    }
    
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        return begin_.Get() + GetSize();
    }
    
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return begin_.Get();
    }
    
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return begin_.Get() + GetSize();
    }
    
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin_.Get();
    }
    
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return begin_.Get() + GetSize();
    }
    
    void swap(SimpleVector& other) {
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
