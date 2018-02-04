//
//  rbt.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 24/03/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "common.h"
#include "rbt.hpp"
#include <array>
#include "rnd.h"

using namespace std;

namespace RBT {

void testRBT()
{
	const int N = 200;
	array<char, N> v;
	
	for( int i = 0; i < N; i++) {
		v[i] = 'A' + random(0, 58);
	}
	
	cout << v;
	
	RBTree<char> tree;
	for( char& c : v) {
		tree.insert(c);
	}
	tree.show();

	array<char, N> d = v;
	randomize(d);

	cout << d;
	
	for( char& c : d) {
		tree.remove(c);
	}

    tree.removeAll();
}

}
