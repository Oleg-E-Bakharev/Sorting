//
//  timSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 11/10/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "timSort.hpp"
#include <vector>
#include <chrono>
#include <random>
#include <stdlib.h>
#include "quickSort.hpp"
#include "common.h"

int compare(const void* a, const void* b) {
	return (*(int*)a - *(int*)b);
}

void testTimSort()
{
    using namespace std;
	using namespace chrono;
//    vector<int> v {1, 7, 8, 6, 9, 4, 5, 3, 2, 0};
//	vector<int> v {0, 1, 4, 4, 4, 4, 4, 4, 8, 9};

//    vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21 };
//	vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20};
//	randomize(v);
//    vector<int> v {8, 18, 5, 16, 19, 20, 13, 2, 4, 10, 6, 9, 12, 15, 7, 11, 3, 17, 1, 14};
	//	vector<int> v {4, 6, 10, 18, 3, 2, 15, 8, 9, 14, 7, 20, 12, 19, 13, 17, 1, 16, 11, 5};
//    for( int i : v) { cout << i << ", "; }; cout << endl;
//        vector<int> v {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18 , 19, 20, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21 };

    std::vector<int> v(100000, 0);
    auto engine = std::default_random_engine{ std::random_device{}() };
	
    std::generate(v.begin(), v.end(), engine);
	
//    std::shuffle(v.begin(), v.end(), engine);
//    auto startQSO = high_resolution_clock::now();
//    quickSort(v);
//    auto performanceQSO = duration_cast<microseconds>( high_resolution_clock::now() - startQSO ).count();
//    std::cout << "quickSort old performance: " << performanceQSO << endl;
//    if(!std::is_sorted(v.begin(), v.end())) {
//        abort();
//    }

//    std::shuffle(v.begin(), v.end(), engine);
//	std::cout << "quickSort performance: " << measureTime([&v]{ quickSort(v.begin(), v.end()); }) << endl;
//    if(!std::is_sorted(v.begin(), v.end())) {
//        abort();
//    }
	std::cout << "quickSort performance: " << measureSort(v, engine, [&v]{ quickSort(v.begin(), v.end()); }) << endl;
	std::cout << "fast quickSort performance: " << measureSort(v, engine, [&v]{ fastSort(v.begin(), v.end()); }) << endl;
	std::cout << "qsort performance: " << measureSort(v, engine, [&v]{ qsort(v.data(), v.size(), sizeof(int), compare); }) << endl;
	std::cout << "timSort performance: " << measureSort(v, engine, [&v]{ timSort(&v.front(), (&v.back())+1); }) << endl;
	std::cout << "std::sort performance: " << measureSort(v, engine, [&v]{ std::sort(v.begin(), v.end()); }) << endl;
	std::cout << "std::stable_sort performance: " << measureSort(v, engine, [&v]{ std::stable_sort(v.begin(), v.end()); }) << endl;
}
