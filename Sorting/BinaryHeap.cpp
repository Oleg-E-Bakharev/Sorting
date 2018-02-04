//
//  BinaryHeap.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 18.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include <iostream>
#include "BinaryHeap.hpp"

using namespace std;

void testBinaryHeap() {
	BinaryHeap<int> h;
	h.insert({1,3,6,2,-4,3,0});
	while (h.size() > 0) {
		cout << h.extractMax() << ", ";
	}
	cout << endl;
}