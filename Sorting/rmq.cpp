//
//  rmq.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 09/01/17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "rmq.hpp"
#include <vector>
#include "common.h"
#include <iostream>

using namespace std;

void testRMQ()
{
    vector<int> v {3, 8, 6, 4, 2, 5, 9, 0, 7, 1};
    cout << v;
    auto r = makeRmq(v);
    cout << "\nsize 2:\n";
    cout << r.query(0, 1) << endl;
    cout << r.query(1, 2) << endl;
    cout << r.query(2, 3) << endl;
    cout << r.query(3, 4) << endl;
    cout << r.query(4, 5) << endl;
    cout << r.query(5, 6) << endl;
    cout << r.query(6, 7) << endl;
    cout << r.query(7, 8) << endl;
    cout << r.query(8, 9) << endl;

    cout << "\nsize 3:\n";
    cout << r.query(0, 2) << endl;
    cout << r.query(1, 3) << endl;
    cout << r.query(2, 4) << endl;
    cout << r.query(3, 5) << endl;
    cout << r.query(4, 6) << endl;
    cout << r.query(5, 7) << endl;
    cout << r.query(6, 8) << endl;
    cout << r.query(7, 9) << endl;
    
    cout << "\nsize 4:\n";
    cout << r.query(0, 3) << endl;
    cout << r.query(1, 4) << endl;
    cout << r.query(2, 5) << endl;
    cout << r.query(3, 6) << endl;
    cout << r.query(4, 7) << endl;
    cout << r.query(5, 8) << endl;
    cout << r.query(6, 9) << endl;
    
    cout << "\nsize 5:\n";
    cout << r.query(0, 4) << endl;
    cout << r.query(1, 5) << endl;
    cout << r.query(2, 6) << endl;
    cout << r.query(3, 7) << endl;
    cout << r.query(4, 8) << endl;
    cout << r.query(5, 9) << endl;

    cout << "\nsize 6:\n";
    cout << r.query(0, 5) << endl;
    cout << r.query(1, 6) << endl;
    cout << r.query(2, 7) << endl;
    cout << r.query(3, 8) << endl;
    cout << r.query(4, 9) << endl;
    
    cout << "\nsize 7:\n";
    cout << r.query(0, 6) << endl;
    cout << r.query(1, 7) << endl;
    cout << r.query(2, 8) << endl;
    cout << r.query(3, 9) << endl;

    cout << "\nsize 8:\n";
    cout << r.query(0, 7) << endl;
    cout << r.query(1, 8) << endl;
    cout << r.query(2, 9) << endl;

    cout << "\nsize 9:\n";
    cout << r.query(0, 8) << endl;
    cout << r.query(1, 9) << endl;

    cout << "\nsize 10:\n";
    cout << r.query(0, 9) << endl;
}
