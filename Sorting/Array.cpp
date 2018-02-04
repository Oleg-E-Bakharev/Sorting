//
//  Array.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 14/09/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "Array.hpp"

using namespace std;

void testArray()
{
    Array<int> a = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Array<int> b = a;
    a.reserve(13);
    b = a;
    cout << b;
}
