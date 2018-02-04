//
//  eratosfen.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 10/08/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef eratosfen_hpp
#define eratosfen_hpp

#include <vector>
#include <iostream>

// Поиск простых числе до заданного предела.
class Eratosfen {
    std::vector<bool> _compound; // массив флажков простоты числа.
    std::vector<size_t> _primes; // массив найденных простых чисел.
    
public:
    // Конструктор принимает максимальное число до которого искать простые числа.
    Eratosfen(size_t max) : _compound(max) {
        _compound[0] = _compound[1] = true;
        size_t current = 2;
        for (;current < max; ++current) {
            if (!_compound[current]) {
                _primes.push_back(current);
                // Заполнение решета с проверкой переполнения.
                for (size_t i = current * current; i < max; i += current) {
                    _compound[i] = true;
                }
            }
        }
    }
    
    // Проверка числа на простоту.
    bool isPrime(size_t k) const {
        return !_compound.at(k); // at - проверяет попадание индекса в границы массива.
    }
    
    // Обращение по индексу.
    size_t operator[](size_t index) const { return _primes.at(index); }
    
    // Оператор вывода в поток.
    friend std::ostream& operator << (std::ostream& os, const Eratosfen& e) {
        os << "Eratosfen to " << e._compound.size() << " found " << e._primes.size() << " prime numbers:\n";
        for( size_t i : e._primes) {
            os << i << "\n";
        }
        return os;
    }
};

class PrimeNumbersGenerator {
    size_t _max = 0;
    size_t _prime = 1;
    std::vector<bool> _compound;
public:
    PrimeNumbersGenerator(size_t max):_max(max), _compound(max) {}
    
    size_t next() {
        size_t prev = _prime;
        // Если у нас уже случилось переполнение, то всегда будет возвращаться число -1 в беззнаковом предствалении
        for (size_t i = _prime + 1; i < _max; i++) {
            if (!_compound[i]) {
                _prime = i;
                // Заполнение решета с проверкой переполнения.
                for (size_t i = _prime * _prime; i < _max; i += _prime) {
                    _compound[i] = true;
                }
                return prev;
            }
        }
        _prime = -2;
        return prev + 1;
    }
    
    operator bool() const {
        return _prime != -2;
    }
    
    friend std::ostream& operator << (std::ostream& os, PrimeNumbersGenerator& prime) {
        os << prime.next();
        return os;
    }
};

int testEratosfen(size_t n);

#endif /* eratosfen_hpp */
