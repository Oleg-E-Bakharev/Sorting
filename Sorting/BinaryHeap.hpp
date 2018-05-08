//
//  BinaryHeap.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 18.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef BinaryHeap_hpp
#define BinaryHeap_hpp

#include <iostream>
#include <algorithm>
//#include "Stack.hpp" // http://rextester.com/IRSM3025
#include <vector>
#include <initializer_list>

// Невозрастающая двоичная прирамида и OrderedSet не ней.
template <typename T, typename Stack = std::vector<T> > class BinaryHeap {
	Stack _data;
	
	size_t parent_(size_t k) {
		return (k-1) / 2; // -1 потому что элементы индексируются с нуля.
	}
		
	// Восстанавливает пирамиду после повышения приоритета элемента k.
	void fixUp_(size_t k) {
		// p - parent.
		for (size_t p = parent_(k); k > 0 && _data[k] > _data[p]; k = p, p = parent_(k)) {
			std::swap(_data[k], _data[p]);
		}
	}
	
	// Восстанавливает пирамиду после понижения приоритета элемента k.
	void fixDown_(size_t k) {
		// c - left child of k
		size_t n = size();
		while (k * 2 + 1 < n) {
			size_t c = k * 2 + 1;
			if ( c < n - 1 && _data[c + 1] > _data[c]) {
				c++; // c - right child of k
			}
			if (_data[c] <= _data[k]) {
				break;
			}
			std::swap(_data[k], _data[c]);
			k = c;
		}
	}
	
	// Создает из неупорядоченного массива кучу. O(n)
    // https://neerc.ifmo.ru/wiki/index.php?title=Двоичная_куча
	void fixHeap_() {
		for (size_t k = size() / 2; k != -1; k--) {
			fixDown_(k);
		}
	}

public:
	
	void insert(T t) {
		_data.pushTail(t);
		fixUp_(size());
	}
	
	void insert(std::initializer_list<T>&& il) {
		for (T t : il) {
			_data.push_back(t);
		}
		fixHeap_();
	}
	
	T extractMax() {
		std::swap(_data[0], _data[size()-1]);
        T max = _data.back();
		_data.pop_back();
		fixDown_(0);
		return max;
	}
	
	// На всякий случай. Если у элемента с номером k поменялся приоритет, то он встанет на свое место.
	void changePriority(size_t k) {
		assert(k < size());
		if (_data[parent_(k)] < _data[k]) {
			fixUp_(k); // Повышение приоритета.
		} else {
			fixDown_(k); // Понижение приоритета.
		}
	}
	
	size_t size() const { return _data.size(); }
};

void testBinaryHeap();

#endif /* BinaryHeap_hpp */
