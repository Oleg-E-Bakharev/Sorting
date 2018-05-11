//
//  ImplicitTreap.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 10/05/2018.
//  Copyright © 2018 Oleg Bakharev. All rights reserved.
//

#include "ImplicitTreap.hpp"

void testImplicitTreap()
{
    ImplicitTreap<int> tree{0,1,2,3,4,5,6,7,8,9};
    auto treeInfo = tree.info();
    std::cout << treeInfo;
    int i = tree[0];
    assert(i == 0);
    i = tree[9];
    assert(i == 9);
    
//    for (int i = 0; i < 10; i++) {
        tree.removeAt(5);
        std::cout << treeInfo;
        tree.insertAt(5, 5);
        std::cout << treeInfo;
//    }
    tree.removeAt(0);
    std::cout << treeInfo;
    tree.insertAt(0, 0);
    std::cout << treeInfo;
    
    tree.removeAt(9);
    std::cout << treeInfo;
    tree.insertAt(9, 9);
    std::cout << treeInfo;
}
