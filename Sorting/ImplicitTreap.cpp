//
//  ImplicitTreap.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 10/05/2018.
//  Copyright © 2018 Oleg Bakharev. All rights reserved.
//

#include "ImplicitTreap.hpp"

using namespace std;

void testImplicitTreap()
{
    ImplicitTreap<int> tree{0,1,2,3,14,5,6,7,8,9};
    cout << tree;
    auto treeInfo = tree.info();
    cout << treeInfo;
    int i = tree[0];
    assert(i == 0);
    i = tree[9];
    assert(i == 9);
    
    cout << "remove 5\n";
    tree.removeAt(5);
    std::cout << treeInfo;
    
    cout << "insert 5\n";
    tree.insertAt(5, 5);
    std::cout << treeInfo;

    cout << "remove 0\n";
    tree.removeAt(0);
    std::cout << treeInfo;
    
    cout << "insert 0\n";
    tree.insertAt(0, 0);
    std::cout << treeInfo;
    
    cout << "remove 9\n";
    tree.removeAt(9);
    std::cout << treeInfo;
    
    cout << "insert 9\n";
    tree.insertAt(9, 9);
    std::cout << treeInfo;
    
    cout << tree;
    
    cout << "shift left 4\n";
    tree.shiftLeft(4);
    std::cout << tree;

    cout << "shift right 4\n";
    tree.shiftRight(4);
    std::cout << tree;

    cout << "\nreverse 1, 9\n";
    tree.reverse(1, 9);
    std::cout << treeInfo;
    std::cout << tree;
    std::cout << treeInfo;

    cout << "reverse 1, 9\n";
    tree.reverse(1, 9);
    std::cout << treeInfo;
    std::cout << tree;
    std::cout << treeInfo;
    

    cout << "reverse\n";
    tree.reverse();
    std::cout << treeInfo;
    std::cout << tree;
    std::cout << treeInfo;
}
