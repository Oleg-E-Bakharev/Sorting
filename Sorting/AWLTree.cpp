//
//  AWLTree.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/11/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "AWLTree.hpp"
#include <random>
#include "assert.h"

void testAWLTree()
{
    using namespace std;
    random_device rd;
    auto random = default_random_engine(rd());
    
	char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    
    AWLTree<char> tree;
    auto treeInfo = tree.info();
    
    shuffle(begin(C), end(C)-1, random);
	
    for( char c : C ) {
        cout << c;
    }
    cout << endl;
    
    for( char c : C ) {
        if (!c)continue;
        cout << "inserting " << c << " =============================\n";
        tree.insert(c);
        treeInfo.info(cout);
    }
    
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
    //tree.insert(C, N);
    
    shuffle(begin(C), end(C) - 1, random);
//    reverse(begin(C), end(C)-1);
    
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
