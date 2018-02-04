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
//template <typename ContainerType>
//void insertionSort( ContainerType& a )
//{
//    for ( auto i = 1; i < a.size(); i++ ) {
//        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
//        auto key = a[i];
//        auto j = i - 1;
//        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
//        while ( j >= 0 && a[j] > key ) {
//            a[j + 1] = a[j];
//            j--;
//        }
//        a[j+1] = key;
//    }
//}

/// Сортировка вставками.
// Эта сортировка сожержит ошибку, поскольку может обращаться к указателю до начала буффера массива.
//template <typename T>
//void insertionSort( T* begin, T* end )
//{
//    for ( T* i = begin + 1; i !=  end; i++ ) {
//        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
//        T key = *i;
//        T* j = i - 1;
//        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
//        while ( j != begin - 1 && *j > key ) {
//            *(j + 1) = *j;
//            j--;
//        }
//        *(j+1) = key;
//    }
//}

///// Сортировка вставками.
//template <typename T>
//void insertionSort( T* begin, T* end )
//{
//    for ( T* i = begin + 1; i !=  end; i++ ) {
//        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
//        T key = *i;
//        T* j = i;
//        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
//        while ( j != begin && *(j-1) > key ) {
//            *(j) = *(j-1);
//            j--;
//        }
//        *(j) = key;
//    }
//}

/// Сортировка вставками.
template <typename It>
void insertionSort( It begin, It end )
{
    for ( It i = begin + 1; i != end; i++ ) {
        // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
        auto key = *i;
        It j = i;
        // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
        while ( j != begin && *(j-1) > key ) {
            *j = *(j-1);
            j--;
        }
        *j = key;
    }
}

#endif /* defined(__InsertionSort__insertionSort__) */
