//
//  sparceVector.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/10/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef sparceVector_hpp
#define sparceVector_hpp

#include <iostream>
#include <map>
#include <deque>
#include <initializer_list>
#include <functional>
#include <assert.h>

// Разреженный вектор. Может индексироваться значениями на всём диапазоне size_t.
// Потребление памяти O(N). Но не менее 4KB.
// Размер устанавливается как количество хранимых значений.
// Произвольный доступ - O(Log(N)).
// Итерация между соседними элементами - O(Log(N)).
// Прим. Если заменить контейнер значений с map на unordered map - получим произвольный доступ за O(1) но лишимся
// возможности итерации по соседним элементами.
template <typename T> class SparseVector {
    
    template <typename TT>
    struct VectorAllocator {
        typedef TT value_type;

        // Не можем использовать deque<TT> поскольку у TT может быть удален деструктор.
        struct Item { char data[sizeof(TT)]; };
        // Проверка этапа компиляции соответствия размеров TT и Item.
        static_assert(sizeof(Item) == sizeof(TT));
        // Дек используем потому что при его росте не инвалидируются указатели на его элементы.
        // При удалении аллокатора, удалится дек и все его элементы.
        std::deque<Item> data;
        
        TT* allocate(std::size_t n) {
            assert(n == 1);
            data.push_back(Item());
            return (TT*)&data.back();
        }
        
        // Необязательный член. И без него всё работает через std::allocator_traits::rebind_alloc
        //template <typename U>
        //struct rebind { using other = VectorAllocator<U>; };

        void deallocate(TT* p, std::size_t) noexcept { /*не используем. После удаления аллокатора - удалим все данные*/ }
    };
    
    // Хранилище значений.
    using Container = std::map<size_t, T, std::less<size_t>, VectorAllocator<std::pair<const size_t, T>>>;
    Container _map;
    
    // Двусторонний итератор.
    class iterator {
        friend class SparseVector;
        using InternalIter = typename Container::iterator;
        using ConstInternalIter = typename Container::const_iterator;
        union {
            InternalIter _it;
            ConstInternalIter _cit;
        };
        
        iterator(const InternalIter& it) : _it(it) {}
        iterator(const ConstInternalIter& cit) : _cit(cit) {}
        
    public:
        const T& operator*() const { return (*_cit).second; }
        T& operator*() { return (*_it).second; }
            
        iterator& operator++() { ++_it; return *this; }
        iterator& operator--() { --_it; return *this; }
            
        iterator operator++(int) { return _it++; }
        iterator operator--(int) { return _it--; }
        
        bool operator==(const iterator& it) const { return _it == it._it; }
        bool operator!=(const iterator& it) const { return _it != it._it; }
    };
    
    using const_iterator = iterator;
    
public:
    using value_type = T;
    
    SparseVector() {}
    
    SparseVector( std::initializer_list<T>&& il ) {
        size_t i = 0;
        for (const T& t : il) _map[i++] = t;
    }
    
    size_t size() const { return _map.size(); }
    
    // Индексаторы. O(log(N))
    const T& operator[](size_t i) const { return _map[i]; }
    T& operator[](size_t i) { return _map[i]; }
    
    // Двунаправленная итерация по индексам. O(Log(N)).
    iterator begin() { return _map.begin(); }
    iterator end() { return _map.end(); }
    const iterator begin() const { return _map.begin(); }
    const iterator end() const { return _map.end(); }

    friend std::ostream& operator<<(std::ostream& os, const SparseVector& v) {
        for (const auto& t : v) os << t << ", "; os << "\n";
        return os;
    }
};

void testSparseVector();

#endif /* sparceVector_hpp */
