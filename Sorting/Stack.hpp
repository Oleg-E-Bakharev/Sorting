//
//  Stack.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 18.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef Stack_hpp
#define Stack_hpp

#include <iostream>
#include "Array.hpp" // http://rextester.com/BLO39784
#include <assert.h>

template <typename T> class Stack : protected Array<T> {
	typedef Array<T> Base;
	size_t _size = 0;
	
public:
	Stack(size_t initialSize = 4) : Base(initialSize) {}
	
	size_t size() const { return _size; }
	
	void pushTail(T t) {
		assert(_size <= Base::size());
		if (_size == Base::size()) {
			this->reserve(_size * 2);
		}
		(*this)[_size++] = t;
	}
	
	T popTail() {
		return (*this)[--_size];
	}
	
	using Base::operator[];
};

#endif /* Stack_hpp */
