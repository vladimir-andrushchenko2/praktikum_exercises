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
    
    // PushBack 1
    {
        SimpleVector<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);
        source.PushBack(4);
        
        assert(source[0] == 1);
        assert(source[1] == 2);
        assert(source[2] == 3);
        assert(source[3] == 4);
        assert(source.GetSize() == 4);
        assert(source.GetCapacity() == 4);
    }
    
    // PushBack 2
    {
        SimpleVector<int> source;
        source.PushBack(1);
        source.PushBack(2);
        source.PushBack(3);
        source.PushBack(4);
        source.PushBack(5);
        assert(source.GetCapacity() == 8);
    }
    
    // PushBack 3
    {
        const SimpleVector<int> source{1, 2, 3};
        SimpleVector<int> target{4, 5, 5, 8};
        
        for (const auto x : source) {
            target.PushBack(x);
        }
        
        assert(target.GetSize() == 7);
        assert(target.GetCapacity() == 8);
    }
    
    // PopBack
    {
        SimpleVector<int> v{1, 2, 3};
        assert(v.IsEmpty() == false);
        
        v.PopBack();
        
        assert(v.GetSize() == 2);
        
        while (!v.IsEmpty()) {
            v.PopBack();
        }
    }
    
    // Insert
    {
        SimpleVector<int> v;
        v.Insert(v.begin(), 1);
        v.Insert(v.begin(), 2);
        v.Insert(v.begin(), 3);
        
        assert(v[0] = 3);
        assert(v[1] = 2);
        assert(v[2] = 1);
    }
    
    // Erase
    {
        SimpleVector<int> v{1, 2, 3};
        v.Erase(v.begin());
        assert(*v.begin() == 2);
    }
    
    // Erase
    {
        SimpleVector<int> v{1, 2, 3};
        while (!v.IsEmpty()) {
            v.Erase(v.begin());
        }
    }
    
    // Erase
    {
        SimpleVector<int> v{1, 2, 3};
        v.Erase(v.end() - 1);
        assert(v.GetSize() == 2);
        assert(v[1] == 2);
    }
    
    // operator==
    {
        SimpleVector<int> v{1, 2, 3};
        SimpleVector<int> v2{1, 2, 3};
        assert(v == v2);
    }
    
    // operator==
    {
        SimpleVector<int> v{1, 2, 3};
        SimpleVector<int> v2{1, 2, 3};
        assert(v == v2);
    }
    
    // Лексикографическое сравнение векторов
    {
        using IntList = SimpleVector<int>;

        assert((IntList{1, 2, 3} < IntList{1, 2, 3, 1}));
        assert((IntList{1, 2, 3} <= IntList{1, 2, 3}));
        assert((IntList{1, 2, 4} > IntList{1, 2, 3}));
        assert((IntList{1, 2, 3} >= IntList{1, 2, 3}));
    }
    
    // Проверка списков на равенство и неравенство
    {
        SimpleVector<int> vector_1;
        vector_1.PushBack(1);
        vector_1.PushBack(2);

        SimpleVector<int> vector_2;
        vector_2.PushBack(1);
        vector_2.PushBack(2);
        vector_2.PushBack(3);

        SimpleVector<int> vector_1_copy;
        vector_1_copy.PushBack(1);
        vector_1_copy.PushBack(2);

        SimpleVector<int> empty_vector;
        SimpleVector<int> another_empty_vector;

        // Список равен самому себе
        assert(vector_1 == vector_1);
        assert(empty_vector == empty_vector);

        // Списки с одинаковым содержимым равны, а с разным - не равны
        assert(vector_1 == vector_1_copy);
        assert(vector_1 != vector_2);
        assert(vector_2 != vector_1);
        assert(empty_vector == another_empty_vector);
    }
    
    // PushBack
       {
           SimpleVector<int> v(1);
           v.PushBack(42);
           assert(v.GetSize() == 2);
           assert(v.GetCapacity() >= v.GetSize());
           assert(v[0] == 0);
           assert(v[1] == 42);
       }

       // Если хватает места, PushBack не увеличивает Capacity
       {
           SimpleVector<int> v(2);
           v.Resize(1);
           const size_t old_capacity = v.GetCapacity();
           v.PushBack(123);
           assert(v.GetSize() == 2);
           assert(v.GetCapacity() == old_capacity);
       }

       // PopBack
       {
           SimpleVector<int> v{0, 1, 2, 3};
           const size_t old_capacity = v.GetCapacity();
           const auto old_begin = v.begin();
           v.PopBack();
           assert(v.GetCapacity() == old_capacity);
           assert(v.begin() == old_begin);
           assert((v == SimpleVector<int>{0, 1, 2}));
       }

       // Конструктор копирования
       {
           SimpleVector<int> numbers{1, 2};
           auto numbers_copy(numbers);
           assert(&numbers_copy[0] != &numbers[0]);
           assert(numbers_copy.GetSize() == numbers.GetSize());
           for (size_t i = 0; i < numbers.GetSize(); ++i) {
               assert(numbers_copy[i] == numbers[i]);
               assert(&numbers_copy[i] != &numbers[i]);
           }
       }

//       // Сравнение
//       {
//           assert((SimpleVector{1, 2, 3} == SimpleVector{1, 2, 3}));
//           assert((SimpleVector{1, 2, 3} != SimpleVector{1, 2, 2}));
//
//           assert((SimpleVector{1, 2, 3} < SimpleVector{1, 2, 3, 1}));
//           assert((SimpleVector{1, 2, 3} > SimpleVector{1, 2, 2, 1}));
//
//           assert((SimpleVector{1, 2, 3} >= SimpleVector{1, 2, 3}));
//           assert((SimpleVector{1, 2, 4} >= SimpleVector{1, 2, 3}));
//           assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 3}));
//           assert((SimpleVector{1, 2, 3} <= SimpleVector{1, 2, 4}));
//       }

       // Обмен значений векторов
       {
           SimpleVector<int> v1{42, 666};
           SimpleVector<int> v2;
           v2.PushBack(0);
           v2.PushBack(1);
           v2.PushBack(2);
           const int* const begin1 = &v1[0];
           const int* const begin2 = &v2[0];

           const size_t capacity1 = v1.GetCapacity();
           const size_t capacity2 = v2.GetCapacity();

           const size_t size1 = v1.GetSize();
           const size_t size2 = v2.GetSize();

           static_assert(noexcept(v1.swap(v2)));
           v1.swap(v2);
           assert(&v2[0] == begin1);
           assert(&v1[0] == begin2);
           assert(v1.GetSize() == size2);
           assert(v2.GetSize() == size1);
           assert(v1.GetCapacity() == capacity2);
           assert(v2.GetCapacity() == capacity1);
       }

       // Присваивание
       {
           SimpleVector<int> src_vector{1, 2, 3, 4};
           SimpleVector<int> dst_vector{1, 2, 3, 4, 5, 6};
           dst_vector = src_vector;
           assert(dst_vector == src_vector);
       }

       // Вставка элементов
       {
           SimpleVector<int> v{1, 2, 3, 4};
           v.Insert(v.begin() + 2, 42);
           assert((v == SimpleVector<int>{1, 2, 42, 3, 4}));
       }

       // Удаление элементов
       {
           SimpleVector<int> v{1, 2, 3, 4};
           v.Erase(v.cbegin() + 2);
           assert((v == SimpleVector<int>{1, 2, 4}));
       }
    
    std::cout << "test2 finished" << std::endl;
}
