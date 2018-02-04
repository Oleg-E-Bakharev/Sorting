//
//  Array.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 14/09/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef Array_hpp
#define Array_hpp

#include <iostream>
#include <algorithm>
#include <assert.h>
#include <initializer_list>

// Динамический массив с расширением. При расширении инвалидируются указатели на элементы.
template <typename T> class Array {
    size_t _size; // Размер буфера.
    T* _buffer; // Буфер массива.
    
    // Пересоздание своего буффера с новым размером с копированием данных из другого (может быть из своего же).
    void copy_(T* buffer, size_t size, size_t newSize) {
        _size = newSize;
        T* oldBuffer = _buffer;
        _buffer = new T[_size]; // Это уже newSize
        // Применяем std::copy вместо memcpy так-как в случае массива не POD типов у
        // них будут вызывать конструкторы копирования.
        std::copy(buffer, buffer + size, _buffer);
        std::fill(_buffer + size, _buffer + _size, T()); // Обнуляем остаток.
        delete[] oldBuffer;
    }
    
public:
    static const size_t MaxSize = 1000000;
    
    // Конструктор по-умолчанию.
    Array(size_t size = 10) : _size(size) {
        assert(_size > 0 && _size <= MaxSize);
        _buffer = new T[size];
    }
    
    // Конструктор списком инициализации.
    Array(std::initializer_list<T> il) : Array(il.size()) {
        assert(_size > 0 && _size <= MaxSize);
        size_t i = 0;
        for( T t : il) {
            (*this)[i++] = t;
        }
    }
    
    // Индексаторы
    T& operator[](size_t i) { return _buffer[i]; }
    T operator[](size_t i) const { return _buffer[i]; }
    
    // Размер массива.
    size_t size() const { return _size; }
    
    void reserve(size_t maxSize) {
        assert(maxSize <= MaxSize);
        if (maxSize > _size) {
            size_t newsize = _size;
            while( maxSize > newsize ) newsize *= 2;
            copy_(_buffer, _size, newsize);
        }
    }
    
    // Правило трех.
    Array(const Array& a) : _buffer(nullptr) { copy_(a._buffer, a._size, a._size); }
    Array& operator=(const Array& a) { copy_(a._buffer, a._size, a._size); return *this; }
    ~Array() { delete[] _buffer; }
    
    // Правило пяти.
    Array(Array&& a) : _size(a._size), _buffer(a._buffer) { a._size = 0; a._buffer = nullptr; }
    Array& operator=(Array&& a) { _size = a._size, _buffer = a._buffer; a._size = 0; a._buffer = nullptr; return *this; }

    // Итераторы для range-based-for.
    T* begin() { return _buffer; }
    T* end() { return _buffer + _size; }
    
    // Вывод содержимого в поток.
    friend std::ostream& operator<<(std::ostream& os, Array& a) {
        for (auto t : a) {
            os << t << " ";
        }
        return os;
    }
};

void testArray();

#endif /* Array_hpp */
