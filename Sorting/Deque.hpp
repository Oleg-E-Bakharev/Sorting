//
//  Deque.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 15/09/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef Deque_hpp
#define Deque_hpp

#include <iostream>
#include <vector>
#include <assert.h>
#include <initializer_list>
// Двусторонняя очередь с расширением, произвольным доступом к элементам и без инвалидации указателей на элементы.
// Push - амортизированная O(1). Pop - O(1). Произвольный доступ - O(1).
// Реализована на динамическом массиве (векторе) цепочек элементов фиксированной длинны.
// Инвариант:
// Если _head == _tail то _size == 0. (1)
// Если цепочка заполнилась до конца, то если следующая цепочка занята (!=nullptr), то расширяемся начиная с нее, иначе занимаем ее (выделяем память на ней). (2)
// Если цепочка очистилась - освобождаем ее и устанавливаем в nullptr. (3)
// Если массив цепочек заполнен до конца - то если начало обнуленное - то используем его, иначе расширяемся.
template <typename T> class Deque {
    
    template<class K>
    using Array = std::vector<K>;
    
    using Chain = Array<T>*;
    Array<Chain> _chains; // Массив цепочек элементов.
    
    size_t _head;
    size_t _tail;
    size_t _size;
    const size_t ChainSize = 10;//sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
	
	Chain& chain_(size_t pos) {
        size_t chainIndex = (pos / ChainSize) % _chains.size();
		return _chains[chainIndex];
	}
    
    T& item_(size_t i) {
        return (*chain_(i))[i % ChainSize];
    }
    
    void growChainsAt_(size_t pos) {
        // Расширяем _chains в точке pos. Все блоки после pos сдвигается на размер расширения.
        // Реально pos может указывать либо на первую ячейку цепочки, либо на последнюю.
        // В случае указания на первую - должны сдвинуть вместе с текущей цепочкой. Если на последнюю, то без текущей.
        size_t chainsGrowFrom = (pos + ChainSize / 2) / ChainSize; // С какой цепочки сдвигаем.
        size_t prevChainsSize = _chains.size();
        size_t chainsToMove = prevChainsSize - chainsGrowFrom; // Количество цепочек, которые нужно сдвинуть.
//        std::cout << _chains;
        _chains.resize(_chains.size() * 2);
//        std::cout << _chains;
        size_t chainsMoveFor = _chains.size() - prevChainsSize; // Размер на который нужно сдвинуть цепочки.
        // Сдвигаем цепочки после chainGrowFrom.
        chainsGrowFrom--; // Итерируемся с конца в начало чтобы исключить возможность перезаписать несдвинутые строчки.
        for (size_t i = chainsGrowFrom + chainsToMove; i != chainsGrowFrom ; i--) {
            _chains[chainsMoveFor + i] = _chains[i];
            _chains[i] = nullptr;
        }
//        std::cout << _chains;
        if ( _tail >= pos) {
            _tail += chainsMoveFor * ChainSize;
        }
        if (_head > pos) {
            _head += chainsMoveFor * ChainSize;
        }
    }
    
    size_t decreaseCounter_(size_t counter) {
        if (counter > 0) {
            counter--;
        } else {
            counter = _chains.size() * ChainSize - 1;
        }
        return counter;
    }
    
    size_t increaseCounter_(size_t counter) {
        if (counter < _chains.size() * ChainSize - 1) {
            counter++;
        } else {
            counter = 0;
        }
        return counter;
    }
    
    void pushAt_(size_t pos, T t) {
        Chain& chain = chain_(pos);
        if (chain == nullptr) {
            chain = new Array<T>(ChainSize);
//            std::cout << _chains;
        }
        (*chain)[pos % ChainSize] = t;
    }
    
    bool checkGrowChains_(size_t prevPos, size_t nextPos) {
        Chain prevChain = chain_(prevPos);
        Chain nextChain = chain_(nextPos);
        if (prevChain == nextChain || nextChain == nullptr) {
            return false;
        }
        growChainsAt_(prevPos);
        return true;
    }
    
    void checkDestroyChain_(size_t pos) {
        if (pos % ChainSize == 0) {
            Chain& chain = chain_(pos);
            if ( chain != nullptr) {
                delete chain;
                chain = nullptr;
//                std::cout << _chains;
            }
        }
    }
    
    void copy_(const Deque& d) {
        _chains = d._chains;
        _head = d._head;
        _tail = d._tail;
        _size = d._size;
        for (auto& chain : _chains) {
            chain = new Array<T>(*chain);
        }
    }
    
    void move_(Deque&& d) {
        _chains = std::move(d._chains);
        _head = d._head; d._head = 0;
        _tail = d._tail; d._tail = 0;
        _size = d._size; d._size = 0;
    }
    
public:
    Deque() : _chains(2), _head(0), _tail(0), _size(0) {
        for (Chain& chain : _chains) {
            chain = nullptr;
        }
    }
    
    Deque(std::initializer_list<T>&& il) : Deque() {
        for(T t : il) {
            pushHead(t);
        };
    }
    
    size_t size() const {return _size;}
    
    T& operator[](size_t i) {
        assert(i < _size);
        return item_(_tail + i);
    }
    const T& operator[](size_t i) const {
        assert( i < _size);
        return item_(_tail + i);
    }
	
	void pushHead(T t) {
        pushAt_(_head, t);
        size_t newHead = increaseCounter_(_head);
        if (checkGrowChains_(_head, newHead) ) {
            _head = increaseCounter_(_head);
        } else {
            _head = newHead;
        }
        _size++;
	}
	
	T popHead() {
        assert(_tail != _head);
        _head = decreaseCounter_(_head);
        T t = item_(_head);
        checkDestroyChain_(_head);
        _size--;
        return t;
	}
	
	void pushTail(T t) {
        size_t newTail = decreaseCounter_(_tail);
        if (checkGrowChains_(_tail, newTail)) {
            _tail = decreaseCounter_(_tail);
        } else {
            _tail = newTail;
        }
        _size++;
        pushAt_(_tail, t);
        if(_head == _tail) {
            // Нарушено св-во 1) инварианта (путем pushHead от начала до нарушения св-ва 1).
            growChainsAt_(_tail);
        }
	}
	
	T popTail() {
        assert(_tail != _head);
        T t = item_(_tail);
        if (_tail + 1 >= ChainSize) {
            checkDestroyChain_(_tail + 1 - ChainSize);
        }
        _tail = increaseCounter_(_tail);
        _size--;
        return t;
    }
	
    ~Deque() {
        for (auto chain : _chains) { delete chain; }
    }
    
    // Вспомогательный код.
    
    // Правило трех.
    Deque(const Deque& d) { copy_(d); }
    Deque& operator=(const Deque& d) { copy_(d); return *this; }
    
    // Правило пяти.
    Deque(Deque&& d) { move_(d); }
    Deque& operator=( Deque&& d) { move_(d); return *this; }
    
    // Последовательный итератор для range-based-for.
    friend class DequeIterator;
    class DequeIterator {
        Deque& _deque;
        size_t _pos;
    public:
        DequeIterator( Deque& deque, size_t pos) : _deque(deque), _pos(pos) {}
        DequeIterator& operator++() { _pos++; return *this; }
        bool operator!=(const DequeIterator& di) const { return _pos != di._pos; }
        T& operator*() { return _deque[_pos]; }
    };

    // Итераторы для range-based-for.
    DequeIterator begin() { return DequeIterator(*this, 0); }
    DequeIterator end() { return DequeIterator(*this, _size); }
    
    // Оператор вывода очереди в поток.
    friend std::ostream& operator<<(std::ostream& os, Deque& deque) {
//        std::cout << deque._chains;
        os << deque.size() << ":{";
        for (T& t : deque) {
            os << t << ", ";
//            assert(t == deque[i++]); // правильность работы индексаторов.
        }
        os << "}\n";
        return os;
    }
};

void testDeque();

#endif /* Deque_hpp */
