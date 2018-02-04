//
//  CountBinaryTree2.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 03/11/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "BinaryTree2.hpp"
#include <random>
#include "assert.h"

void testBinaryTree2()
{
    using namespace std;
    random_device rd;
    auto random = default_random_engine(rd());
    
    char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int N = sizeof(C)/sizeof(*C) - 1; // Завершающий 0 отбрасываем.
    
    BinaryTree2<char> tree;
    auto treeInfo = tree.info();
    
    shuffle(C, C + N, random);
    
    for( char c : C ) {
        cout << c;
    }
    cout << endl;
    
    tree.insert(C, N);
    
    treeInfo.info(cout);
    cout << "===========\nBalanced:";
    tree.balance();
    treeInfo.info(cout);
    
    shuffle(C, C + N, random);
    
    for( char c : C ) {
        cout << c;
    }
    cout << endl;
    
    for( char c : C ) {
        if (!c)continue;
        cout << "removing " << c << " =============================\n";
        size_t size = treeInfo.size();
        bool result = tree.remove(c);
        treeInfo.info(cout);
        assert(result);
        assert(treeInfo.size() == size - 1);
    }
}
