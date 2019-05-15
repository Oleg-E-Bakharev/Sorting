//
//  lcaBinaryLifting.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 12/01/17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "lcaBinaryLifting.hpp"
#include "common.h"
#include <iostream>

using namespace std;

void testLcaBinaryLifting()
{
    //                     0  1  2  3  4  5  6  7  8  9
//    std::vector<size_t> v {1, 3, 3, 4, 4, 4, 5, 5, 7, 6};
//    std::vector<size_t> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 9};
    std::vector<size_t> v {1, 2, 3, 4, 5, 5, 5, 6, 7, 8, 9};
    
    auto lca = makeLcaBL(v);
    
//    lca.query(0, 3);
//    lca.query(0, 8);
//    lca.query(1, 9);
//    lca.query(9, 8);
    lca.query(0, 10);
}
