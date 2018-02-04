//
//  treeTest.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 05.08.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include <iostream>
#include <assert.h>
#include <random>
#include "BinaryTree.hpp"

void testTree()
{
    using namespace std;
    random_device rd;
    auto random = default_random_engine(rd());
    
    //char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";//АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    const int N = sizeof(C)/sizeof(*C) - 1; // Завершающий 0 отбрасываем.
    
    BinaryTree<char> tree;
    auto treeInfo = tree.info();
    
    shuffle(C, C + N, random);
    
    for( char c : C ) {
        cout << c;
    }
    cout << endl;

	tree.insert(begin(C), end(C));
//    for( char c : C ) {
//        if (!c)continue;
//        //        cout << "inserting " << c << " =============================\n";
//        tree.insert(c);
//    }
    treeInfo.info(cout);
    
    //    return ;
    cout << "||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||\n";
    //tree.insert(C, N);
    
    shuffle(C, C + N, random);
    
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
            assert(false);
        }
        prevHeight = height;
        bool result = tree.remove(c);
        treeInfo.info(cout);
        assert(result);
        assert(treeInfo.size() == size - 1);
    }
//    char C[] = "EDGHBFAC";//{ {'E'}, {'D'}, {'H'}, {'B'}, {'F'}, {'A'}, {'C'}, {'G'} };
//    //    CharItem C[] { {'D'}, {'B'}, {'F'}, {'A'}, {'C'}, {'E'}, {'G'} };
//    //    CharItem C[] { {'A'}, {'B'}, {'C'}, {'D'}, {'E'}, {'F'}, {'G'} };
//    //    CharItem C[] { {'A'}, {'G'}, {'B'}, {'F'}, {'C'}, {'E'}, {'D'} };
//    //    CharItem C[] { {'D'}, {'E'}, {'C'}, {'F'}, {'B'}, {'G'}, {'A'} };
//    const int N = sizeof(C)/sizeof(*C) - 1; // Завершающий 0 отбрасываем.
//
//    BinaryTree<char> tree;
//    auto treeInfo = tree.info();
////    assert(t.count() == 0);
//    tree.insert(C, N);
////    cout << tree;
////    assert(tree.count() == N);
//    treeInfo.info(cout);
////    cout << " count = " << tree.count() << endl;
////    cout << " height = " << tree.height() << endl;
//
//    assert(tree.has('D'));
//    assert(tree.has('B'));
//    assert(tree.has('A'));
//    assert(tree.has('C'));
//
//    assert(tree.remove('D'));
//    cout << treeInfo;
//    assert(!tree.has('D'));
//
//    tree.remove('B');
//    cout << treeInfo;
//
//    tree.remove('F');
//    cout << treeInfo;
//
//    tree.remove('E');
//    cout << treeInfo;
//
//    tree.remove('C');
//    cout << treeInfo;
//
//    tree.remove('A');
//    cout << treeInfo;
//
//    tree.remove('G');
//    cout << treeInfo;
//
//    tree.remove('H');
//    cout << treeInfo;
//
////    cout << " count = " << tree.count() << endl;
////    cout << " height = " << tree.height() << endl;
}
