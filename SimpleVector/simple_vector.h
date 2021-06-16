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
        size_ = capacity_ = init.size();
        ArrayPointer<Type> temp(capacity_);
        begin_.swap(temp);
        std::copy(init.begin(), init.end(), begin_.Get());
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
        return size_ == 0;
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
        size_ = 0;
    }
    
    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        if (new_size <= capacity_) {
            size_ = new_size;
            std::fill(end(), begin() + capacity_, Type{});
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
        return begin_.Get() + size_;
    }
    
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        return begin_.Get();
    }
    
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        return begin_.Get() + size_;
    }
    
    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        return begin_.Get();
    }
    
    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        return begin_.Get() + size_;
    }
    
    void swap(SimpleVector& other) {
        std::swap(capacity_, other.capacity_);
        std::swap(size_, other.size_);
        begin_.swap(other.begin_);
    }
    
private:
    size_t size_ = 0;
    size_t capacity_ = 0;
    
    ArrayPointer<Type> begin_;
};
