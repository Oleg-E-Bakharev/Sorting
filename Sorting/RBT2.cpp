//
//  RBT2.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 22/08/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "common.h"
#include "RBT2.hpp"
#include <array>
#include <iostream>
#include "rnd.h"

using namespace std;

void testRBTree2()
{
    const int N = 200;
    array<char, N> v;
    
    for( int i = 0; i < N; i++) {
        v[i] = 'A' + random(0, 26);
    }
    
    cout << v;
    
    RBTree2<char> tree;
    for( char& c : v) {
//        cout << c << endl;
        tree.insert(c);
//        cout << tree;
    }
    tree.info(cout);
    
//    array<char, N> d = v;
//    randomize(d);
//    
//    cout << d;
//    
//    for( char& c : d) {
//        tree.remove(c);
//    }
}