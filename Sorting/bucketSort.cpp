//
//  bucketSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 21.11.15.
//  Copyright (c) 2015 Oleg Bakharev. All rights reserved.
//

#include "bucketSort.h"
#include "list.hpp"
#include "rnd.h"
#include "testSort.hpp"

#include <iostream>
#include <chrono>

using namespace std;
using namespace chrono;

// На входе массив из n равномерно распределенных чисел от 0 до 1
void bucketSort( double* D, int n)
{
    auto la  = ListAllocator<double>(n);
    
    auto lists = new List<double>[n];
    for (int i = 0; i < n; i++) {
        double d = D[i];
        int bucket = int(n * d);
        List<double>& l = lists[bucket];
        l.insert(*la.alloc(d), l.tail());
    }
    
    for (int i = 0; i < n; i++) {
        lists[i].sort();
    }
    
    int k = 0;
    for (int i = 0; i < n; i++) {
        auto l = lists[i];
        auto node = &l.head();
        for (int j = 0, s = l.size(); j < s; j++) {
            D[k++] = node->key;
            node = node->next;
        }
    }
}

void testBucketSort(int n)
{
    auto D = new double[n];
    for (int i = 0; i < n; i++) {
        D[i] = random(0, 10000) / 10000.;
    }
	auto start = high_resolution_clock::now();
    bucketSort(D, n);
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
	std::cout << "bucketSortPerformance: " << performance << endl;
	
    testSort(D, n);

    delete [] D;
}