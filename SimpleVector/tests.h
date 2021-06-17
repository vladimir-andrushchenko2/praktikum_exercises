#pragma once
#include <cassert>
#include <stdexcept>
#include <iostream>

// У функции, объявленной со спецификатором inline, может быть несколько
// идентичных определений в разных единицах трансляции.
// Обычно inline помечают функции, чьё тело находится в заголовочном файле,
// чтобы при подключении этого файла из разных единиц трансляции не возникало ошибок компоновки
inline void Test1() {
    // Инициализация конструктором по умолчанию
    {
        SimpleVector<int> v;
        assert(v.GetSize() == 0u);
        assert(v.IsEmpty());
        assert(v.GetCapacity() == 0u);
    }

    // Инициализация вектора указанного размера
    {
        SimpleVector<int> v(5);
        assert(v.GetSize() == 5u);
        assert(v.GetCapacity() == 5u);
        assert(!v.IsEmpty());
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 0);
        }
    }

    // Инициализация вектора, заполненного заданным значением
    {
        SimpleVector<int> v(3, 42);
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        for (size_t i = 0; i < v.GetSize(); ++i) {
            assert(v[i] == 42);
        }
    }

    // Инициализация вектора при помощи initializer_list
    {
        SimpleVector<int> v{1, 2, 3};
        assert(v.GetSize() == 3);
        assert(v.GetCapacity() == 3);
        assert(v[2] == 3);
    }

    // Доступ к элементам при помощи At
    {
        SimpleVector<int> v(3);
        assert(&v.At(2) == &v[2]);
        try {
            v.At(3);
            assert(false);  // Ожидается выбрасывание исключения
        } catch (const std::out_of_range&) {
        } catch (...) {
            assert(false);  // Не ожидается исключение, отличное от out_of_range
        }
    }
    
    // Очистка вектора
    {
        SimpleVector<int> v(10);
        const size_t old_capacity = v.GetCapacity();
        v.Clear();
        assert(v.GetSize() == 0);
        assert(v.GetCapacity() == old_capacity);
    }
    
    // Изменение размера
    {
        SimpleVector<int> v(3);
        v[2] = 17;
        v.Resize(7);
        assert(v.GetSize() == 7);
        assert(v.GetCapacity() >= v.GetSize());
        assert(v[2] == 17);
        assert(v[3] == 0);
    }
    {
        SimpleVector<int> v(3);
        v[0] = 42;
        v[1] = 55;
        const size_t old_capacity = v.GetCapacity();
        v.Resize(2);
        assert(v.GetSize() == 2);
        assert(v.GetCapacity() == old_capacity);
        assert(v[0] == 42);
        assert(v[1] == 55);
    }
    {
        const size_t old_size = 3;
        SimpleVector<int> v(3);
        v.Resize(old_size + 5);
        v[3] = 42;
        v.Resize(old_size);
        v.Resize(old_size + 2);
        assert(v[3] == 0);
    }
    
    // Итерирование по SimpleVector
    {
        // Пустой вектор
        {
            SimpleVector<int> v;
            assert(v.begin() == nullptr);
            assert(v.end() == nullptr);
        }

        // Непустой вектор
        {
            SimpleVector<int> v(10, 42);
            assert(v.begin());
            assert(*v.begin() == 42);
            assert(v.end() == v.begin() + v.GetSize());
        }
    }
 
    std::cout << "test1 finished" << std::endl;
}

inline void Test2() {
    // copy constructor
    {
        SimpleVector<int> v{1, 2, 3};
        SimpleVector<int> v2(v);
        
        assert(v[0] == v2[0]);
        assert(v[1] == v2[1]);
        assert(v[2] == v2[2]);
        assert(v.GetSize() == v2.GetSize());
    }
    
    // assignment operator
    {
        const SimpleVector<int> source{1, 2, 3};
        SimpleVector<int> target{4, 5, 5, 8};
        
        target = source;
        assert(source[0] == target[0]);
        assert(source[1] == target[1]);
        assert(source[2] == target[2]);
        assert(source.GetSize() == target.GetSize());
    }
    
    std::cout << "test2 finished" << std::endl;
}
