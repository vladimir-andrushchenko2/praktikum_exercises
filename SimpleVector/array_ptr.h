#pragma once

#include <algorithm>

template <typename Type>
class ArrayPointer {
public:
    ArrayPointer() = default;

    // Создаёт в куче массив из size элементов типа Type.
    // Если size == 0, поле raw_ptr_ должно быть равно nullptr
    explicit ArrayPointer(size_t size) {
        if (size == 0) return;
        raw_ptr_ = new Type[size];
    }

    explicit ArrayPointer(Type* raw_ptr) noexcept: raw_ptr_(raw_ptr) {}

    ArrayPointer(const ArrayPointer&) = delete;

    ~ArrayPointer() {
        delete[] raw_ptr_;
        raw_ptr_ = nullptr;
    }

    ArrayPointer& operator=(const ArrayPointer&) = delete;

    // Прекращает владением массивом в памяти, возвращает значение адреса массива
    // После вызова метода указатель на массив должен обнулиться
    [[nodiscard]] Type* Release() noexcept {
        auto temp = raw_ptr_;
        raw_ptr_ = nullptr;
        return temp;
    }

    Type& operator[](size_t index) noexcept {
        return *(raw_ptr_ + index);
    }

    const Type& operator[](size_t index) const noexcept {
        return *(raw_ptr_ + index);
    }

    explicit operator bool() const {
        return raw_ptr_ != nullptr;
    }

    // Возвращает значение сырого указателя, хранящего адрес начала массива
    Type* Get() const noexcept {
        return raw_ptr_;
    }

    // Обменивается значениям указателя на массив с объектом other
    void swap(ArrayPointer& other) noexcept {
        std::swap(raw_ptr_, other.raw_ptr_);
    }

private:
    Type* raw_ptr_ = nullptr;
};
