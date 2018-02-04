//
//  heapSort.h
//  Sorting
//
//  Created by Oleg Bakharev on 25/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef heapSort_h
#define heapSort_h

#include <stdio.h>

// Сортировка пирамидой.
void heapSort(int* a, int n);

class PriorityQueue {
    int* a;
    int n;
    int l;
    
public:
    PriorityQueue(int* a, int n, int l);
    int limit() {return l;}
    int size() {return n;}
    int max() {return *a;}
    int extractMax();
    void insert(int i);
    void increaseKey(int i, int k);
    void decreaseKey(int i, int k);
};

#endif /* heapSort_h */
