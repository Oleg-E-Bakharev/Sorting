//
//  MergeSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 25/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "mergeSort.h"
#include <stdio.h>
//#include <iostream>

//using namespace std;

void insertionSortA( int* a, int size )
{
    for ( auto i = 1; i < size; i++ ) {
        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
        auto key = a[i];
        auto j = i - 1;
        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
        while ( j >= 0 && a[j] > key ) {
            a[j + 1] = a[j];
            j--;
        }
        a[j+1] = key;
    }
}

void mergeABtoC( int* a, int aCount, int* b, int bCount, int* c)
{
    for (int i = 0, j = 0, k = 0; k < aCount + bCount; k++) {
        if (i == aCount) {
            c[k] = b[j++];
        } else if (j == bCount) {
            c[k] = a[i++];
        } else {
            c[k] = (a[i] < b[j]) ? a[i++] : b[j++];
        }
    }
}

void mergeSortAB( int* a, int* b, int l, int r)
{
    if (r == l) {
        return;
    }
    
    int m = (l + r) / 2;
    
    mergeSortAB(b, a, l, m);
    mergeSortAB(b, a, m + 1, r);
    mergeABtoC(b + l, m - l + 1, b + m + 1, r - m, a + l);
}

void mergeSortAB2( int* a, int* b, int l, int r)
{
    if (r - l < 10) {
        insertionSortA(a + l, r - l);
        return;
    }
    
    int m = (l + r) / 2;
    
    mergeSortAB(b, a, l, m);
    mergeSortAB(b, a, m + 1, r);
    mergeABtoC(b + l, m - l + 1, b + m + 1, r - m, a + l);
}

const int maxN = 1000000;

void mergeSort( int* a, int count )
{
    // static вспомогательный массив выделен к моменту запуска программы и не меняется между вызовами.
    static int aux[maxN] = {0};
    
    for (int i = 0; i < count; i++) {
        aux[i] = a[i];
    }
    mergeSortAB(a, aux, 0, count - 1);
}