//
//  sparseVector.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/10/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "sparseVector.hpp"

using namespace std;

void testSparseVector() {
    SparseVector<int> v {0,1,2,3,4,5,6,7,8};
    v[2037] = 32;
    v[65535] = 20;
    cout << v;
}
