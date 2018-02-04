//
//  main.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 05/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include <stdlib.h>
#include "common.h"
#include "performance.h"
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
#include "countingSort.h"
#include "radixSort.hpp"
#include "bucketSort.h"
#include "bstOld.hpp"
#include "rbt.hpp"
#include "matrix.h"
#include "hashTable.hpp"
#include "BinaryTree.hpp"
#include "eratosfen.hpp"
#include "RBT2.hpp"
#include "ForwardList.hpp"
#include "Array.hpp"
#include "Deque.hpp"
#include "BinaryHeap.hpp"

class C {
	void f () {
		using namespace std;
	}
};

using namespace std;

template <typename Container, typename Function>
void measureSort(Container& array, Function func, const char* message ) {
    startPerformanceCounter();
    func(array);
    double performance = getPerformanceCounter();
    std::cout << message << performance << endl;
    testSort(array.data(), array.size());
}

template <typename Container, typename Function>
void measureIntSort(Container& array, Function func, const char* message ) {
    startPerformanceCounter();
    func(array.data(), (int)array.size());
    double performance = getPerformanceCounter();
    std::cout << message << performance << endl;
//    cout << array;
    testSort(array.data(), array.size());
}

void testSorting()
{
    const int N = 10000;
//    auto a = monotonicSequence<array<int, N>>(N, -N/2); //randomSequence<array<int, 100>>(100, -100, 100);
    auto a = monotonicSequence<array<int, N>>(N, 0);
//    std::vector<int> a = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7, 17 };
//    std::array<int, 11> a = {4, 1, 3, 2, 16, 9, 10, 14, 17, 7, 8};

//    std::cout << a.size() << endl << a << endl;
    
    randomize(a);
    measureIntSort(a, bubbleSort, "bubble: ");
    randomize(a);
    measureIntSort(a, selectionSort, "selection: ");
    randomize(a);
    measureSort(a, insertionSort<typeof a>, "insertion: ");
    randomize(a);
    measureIntSort(a, shellSort, "shell: ");
    randomize(a);
    measureIntSort(a, quickSort, "qicksort: ");
    randomize(a);
    measureIntSort(a, mergeSort, "mergeSort: ");
    randomize(a);
    measureIntSort(a, heapSort, "heapSort: ");
//
//    testQueue();
	
//    testListSort(N);
    
    testCountingSort(N, -128, 127);
    
    randomize(a);
    testRadix(a.data(), N);
//
//    testBucketSort(N);
//    RBT::testRBT();
//    testMatrix();
//    testHashTable();
}

int main(int argc, const char * argv[]) {
	randomInit();
	
//    testEratosfen(1000);
	testTree();
//    testRBTree2();
//	testForwardList();
//    testArray();
//    testDeque();
//	testBinaryHeap();
	
	return 0;
}

