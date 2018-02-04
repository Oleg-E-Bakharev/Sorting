//
//  insertionSort.h
//  InsertionSort
//
//  Created by Oleg Bakharev on 06/08/15.
//  Copyright (c) 2015 Oleg Bakharev. All rights reserved.
//

#ifndef __InsertionSort__insertionSort__
#define __InsertionSort__insertionSort__

/// Сортировка вставками.
template <typename ContainerType>
void insertionSort( ContainerType& a )
{
    for ( auto i = 1; i < a.size(); i++ ) {
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

inline void insertionSortI( int* a, int* b )
{
    for ( int* i = a + 1; i != b; i++ ) {
        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
        int key = *i;
        int* j = i - 1;
        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
        while ( j != a && *j > key ) {
            *(j + 1) = *j;
            j--;
        }
        *(j+1) = key;
    }
}

#endif /* defined(__InsertionSort__insertionSort__) */
