//
//  Deque.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 15/09/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "Deque.hpp"

using namespace std;

void testDeque()
{
    Deque<int> d; // {1, 2, 3, 4, 5};
//    d.pushTail(1);
//    for(int i = 0; i < 15; i++) { d.pushTail(i); }
//    cout << d;

    for(int i = 0; i < 25; i++) { d.pushTail(i); }
    cout << d;
    for(int i = 0; i < 15; i++) { d.popHead(); }
    cout << d;
    for(int i = 0; i < 25; i++) { d.pushTail(i + 25); }
    cout << d;
    for(int i = 0; d.size() > 0; i++) { d.popHead(); }
    cout << d;

    for(int i = 0; i < 25; i++) { d.pushHead(i); }
    cout << d;
    for(int i = 0; i < 15; i++) { d.popTail(); }
    cout << d;
    for(int i = 0; i < 25; i++) { d.pushHead(i + 25); }
    cout << d;
    for(int i = 0; d.size() >0; i++) { d.popTail(); }
    cout << d;
}
