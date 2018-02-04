//
//  heapSort.c
//  Sorting
//
//  Created by Oleg Bakharev on 25/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "heapSort.h"
#include "assert.h"

inline void swap(int& i, int& j)
{
    int t = i;
    i = j;
    j = t;
}

// Восстанавливает пирамиду после повышения приоритета элемента k.
inline void fixUp( int* a, int k)
{
    // p - parent.
    for (int p = (k - 1) / 2; k > 0 && a[k] > a[p]; k = p, p = (k - 1) / 2) {
        swap(a[k], a[p]);
    }
}

// Восстанавливает пирамиду после понижения приоритета элемента k.
inline void fixDown( int* a, int n, int k) {
    // c - left child of k
    while (k * 2 + 1 < n) {
        int c = k * 2 + 1;
        if ( c < n - 1 && a[c + 1] > a[c]) {
            c++; // c - right child of k
        }
        if (a[c] <= a[k]) break;
        swap(a[k], a[c]);
        k = c;
    }
}

// Воссоздает из неупорядоченного массива неубывающую пирамиду.
inline void fixHeap( int* a, int n)
{
    for (int k = n / 2; k >= 0; k--) {
        fixDown(a, n, k);
    }
}

// Сортировка пирамидой.
void heapSort(int* a, int n)
{
    fixHeap(a, n);
    while (n > 1) {
        swap(a[0], a[n-1]);
        fixDown(a, --n, 0);
    }
}

PriorityQueue::PriorityQueue(int* a, int n, int l) : a(a), n(n), l(l)
{
    assert(l >= n);
    fixHeap(a, n);
}

int PriorityQueue::extractMax()
{
    assert(n > 0);
    swap(a[0], a[--n]);
    fixDown(a, n, 0);
    return a[n];
}

void PriorityQueue::insert(int i)
{
    assert(n < l);
    a[n] = i;
    fixUp(a, n++);
}

void PriorityQueue::increaseKey(int i, int k)
{
    assert(i < n);
    assert(k >= a[i]);
    a[i] = k;
    fixUp(a, k);

}

void PriorityQueue::decreaseKey(int i, int k)
{
    assert(i < n);
    assert(k <= a[i]);
    a[i] = k;
    fixDown(a, n, k);
}