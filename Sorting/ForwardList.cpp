//
//  ForwardList.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 10.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "ForwardList.hpp"

using namespace std;

void testForwardList()
{
	ForwardList<int> fl;
	
    fl.addTail(1);
    int i = fl.delHead();
    assert(fl.isEmpty());
    
    fl.addHead(1);
    i = fl.delHead();
    assert(fl.isEmpty());
    
    fl.addTail(7);
    fl.addTail(2);
    fl.addTail(6);
    fl.addTail(9);
    fl.addTail(4);
    assert(fl.search(4) == true);
    fl.addHead(3);
    fl.addHead(0);
    fl.delHead();
    fl.addHead(1);
    fl.addTail(-3);
    fl.addTail(12);
    fl.addTail(22);
    fl.addTail(11);
    fl.addTail(-8);

    cout << fl;
    fl.sort();
    cout << fl;
	fl.reverse();
    cout << fl;
}
