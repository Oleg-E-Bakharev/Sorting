//
//  randomBST.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 24/08/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "RandomBST.hpp"

void testRandomBST()
{
    using namespace std;
    random_device rd;
    auto random = default_random_engine(rd());
	
//	char C[] = "0123456789"
    char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	
    RandomBinaryTree<char> tree;
    auto treeInfo = tree.info();
    
    shuffle(begin(C), end(C), random);
    
    for( char c : C ) {
        cout << c;
    }
    cout << endl;
    
    for( char c : C ) {
        if (!c)continue;
//        cout << "inserting " << c << " =============================\n";
        tree.insert(c);
    }
    treeInfo.info(cout);

//    return ;
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
    //tree.insert(C, N);
    
    shuffle(begin(C), end(C), random);
    
    for( char c : C ) {
        cout << c;
    }
    cout << endl;
    
    size_t prevHeight = treeInfo.height();
    for( char c : C ) {
        if (!c)continue;
        cout << "removing " << c << " =============================\n";
        size_t size = treeInfo.size();
        size_t height = treeInfo.height();
        if (height > prevHeight) {
            int i = 0;
        }
        prevHeight = height;
        bool result = tree.remove(c);
        treeInfo.info(cout);
        assert(result);
        assert(treeInfo.size() == size - 1);
    }
}
