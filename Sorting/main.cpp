//
//  main.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 05/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include <stdlib.h>
#include "common.h"
#include <chrono>
#include <algorithm>
#include "testSort.hpp"
#include "rnd.h"
#include "bubbleSort.hpp"
#include "insertionSort.h"
#include "shellSort.h"
#include "selectionSort.hpp"
#include "quickSort.h"
#include "mergeSort.h"
#include "heapSort.h"
#include "queue.hpp"
#include "list.hpp"
#include "quickSort.hpp"
#include "countingSort.h"
#include "radixSort.hpp"
#include "bucketSort.h"
#include "bstOld.hpp"
#include "rbt.hpp"
#include "matrix.h"
#include "hashTable.hpp"
#include "BinaryTree.hpp"
#include "SplayTree.hpp"
#include "RandomBST.hpp"
#include "ImplicitTreap.hpp"
#include "eratosfen.hpp"
#include "RBT2.hpp"
#include "ForwardList.hpp"
#include "Array.hpp"
#include "Deque.hpp"
#include "BinaryHeap.hpp"
#include "timSort.hpp"
#include "BinaryTree2.hpp"
#include "LLRB.hpp"
#include "LLRB2.hpp"
#include "AWLTree.hpp"
#include "BitStream.hpp"
#include "BitMemoryStream.hpp"
#include "rmq.hpp"
#include "lcaBinaryLifting.hpp"
#include "MSD.hpp"
//#include "MSDQSortDebug.hpp"
#include "MSDQSort.hpp"
#include "LSD.hpp"
#include "LSD3.hpp"
#include "sparseVector.hpp"
#include "StringSortCompare.hpp"
#include "MapSet.hpp"
#include "hashArray.hpp"

class C {
	void f () {
		using namespace std;
	}
};

using namespace std;
using namespace std::chrono;

template <typename Container, typename Function>
void measureSort(Container& array, Function func, const char* message ) {
	auto start = high_resolution_clock::now();
    func(array);
    auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
    std::cout << message << performance << endl;
	assert(is_sorted(array.begin(), array.end()));
}

template <typename It, typename Function>
void measureIterSort(It begin, It end, Function func, const char* message ) {
	//    startPerformanceCounter();
//    for(auto i = begin; i != end; i++) cout << *i << ", "; cout << "\n";
	auto start = high_resolution_clock::now();
    
	func(begin, end);
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
	std::cout << message << performance << endl;
	//    testSort(array.data(), array.size());
    if (!is_sorted(begin, end)) {
        for(auto i = begin; i != end; i++) cout << *i << ", ";
        abort();
    }
//    assert(is_sorted(begin, end));
}

template <typename Container, typename Function>
void measureIntSort(Container& array, Function func, const char* message ) {
	auto start = high_resolution_clock::now();
	func(array.data(), (int)array.size());
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
    std::cout << message << performance << endl;
//    cout << array;
//    testSort(array.data(), array.size());
	assert(is_sorted(array.begin(), array.end()));
}

template <typename T, typename Function>
void measureIntSort(T* begin, T* end, Function func, const char* message ) {
	auto start = high_resolution_clock::now();
	func(begin, end);
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
	std::cout << message << performance << endl;
//	for(T* p = begin; p != end; p++) {
//		cout << *p << ", ";
//	}
//	cout << endl;
	assert(is_sorted(begin, end));
//	testSort(array.data(), array.size());
}

template<class Iter>
void sortWrap(Iter begin, Iter end) {
	sort(begin, end);
}

template<class Iter>
void stableSortWrap(Iter begin, Iter end) {
	stable_sort(begin, end);
}

template<class Iter>
void quickSortWrap(Iter begin, Iter end) {
    quickSort(begin, end);
}

void testSorting()
{
    const int N = 10000000;
//    const int N = 10;
//    auto a = monotonicSequence<vector<int>>(N, -N/2); //randomSequence<array<int, 100>>(100, -100, 100);
    auto a = monotonicSequence<vector<int>>(N, 0);
//    std::reverse(&a[N/2], (&a.back()) + 1);
//    std::vector<int> a = { 3, 1, 4, 5, 9, 2, 0, 7, 6, 8,  };
//    std::array<int, 11> a = {4, 1, 3, 2, 16, 9, 10, 14, 17, 7, 8};
	
//	int a[] = {4, 1, 3, 2, 16, 9, 10, 14, 17, 7, 8};

//    std::cout << a.size() << endl << a << endl;
    
//    randomize(a);
//	insertionSort(a, a + 11);
//    for (int i : a) {
//        cout << i << ", ";
//    }
//	cout << endl;
//    measureIntSort(a, bubbleSort, "bubble: ");
//    randomize(a);
//    measureIntSort(a, selectionSort, "selection: ");
//	randomize(a);
//    measureIntSort(&a.front(), (&a.back())+1, insertionSort<int>, "insertion: ");

//    randomize(a);
//    measureIntSort(a, shellSort, "shell: ");
////    randomize(a);
////    measureIntSort(a, quickSort, "quicksort: ");
//    randomize(a);
//    measureIntSort(a, mergeSort, "mergeSort: ");
//    randomize(a);
//    measureIntSort(a, heapSort, "heapSort: ");
//    randomize(a);
//    measureIntSort(&a.front(), (&a.back())+1, timSort<int>, "timSort: ");
//
    randomize(a);
    
    using sortIter = decltype(a)::iterator;
    measureIterSort(a.begin(), a.end(), sortWrap<sortIter>,  "stdSort: ");
//
    randomize(a);
    measureIterSort(a.begin(), a.end(), quickSortWrap<sortIter>, "quickSort: ");

    randomize(a);
    measureIterSort(a.begin(), a.end(), fastSort<sortIter, fatPartition>, "fatSort: ");

    randomize(a);
    measureIterSort(a.begin(), a.end(), fastSort<sortIter>, "fastSort: ");

//    randomize(a);
//    measureIterSort(a.begin(), a.end(), stableSortWrap<sortIter>, "stdStableSort: ");
	//    testQueue();
	
//    testListSort(N);
//	
//    testCountingSort(N, -128, 127);
//    
//    randomize(a);
//    testRadix(a.data(), N);
//
//    testBucketSort(N);
//    RBT::testRBT();
//    testMatrix();
//    testHashTable();
}

int main(int argc, const char * argv[]) {
	randomInit();
//    testCountingSort();
//    testLSD3();
//    testMSD();
    stringSortCompare();
//    testMSDQSort();
//    testSparseVector();
//    testSorting();
//    testTimSort();
//    testEratosfen(1000);
//    testTree();
//    testRandomBST();
//    testImplicitTreap();
//    testSplayTree();
//    testAWLTree();
//	testMapSet();
//    testBinaryTree2();
//	testLLRB();
//    testLLRB2();
//    testRBTree2();
//    testForwardList();
//    testArray();
//    testDeque();
//    testBinaryHeap();
//	testHashTable();
//    testHashArray();
	
//	testBitStream();
//    cout << "=======================================\n";
//    testBitMemoryStream();
    
//    testRMQ();
//    testLcaBinaryLifting();
	return 0;
}

