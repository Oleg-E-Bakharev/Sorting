//
//  quickSort.c
//  Sorting
//
//  Created by Oleg Bakharev on 25/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "quickSort.h"

static inline void swap(int& i, int& j)
{
    int t = i;
    i = j;
    j = t;
}

// Возвращает индекс разбиения i в котором a[k] <= a[i] : l <= k < i;  a[k] >= a[i] : i < k <= r
int partition(int* a, int l, int r)
{
    auto p = a[r];
    int i = l;
    for (size_t j = l; j < r; j++) {
        if (a[j] < p) {
            swap(a[i], a[j]);
            i++;
        }
    }
    swap(a[i], a[r]);
    return i;
}

// Возвращает индекс разбиения i в котором a[k] <= a[i] : l <= k < i;  a[k] >= a[i] : i < k <= r
int partition2(int* a, int l, int r)
{
    int p = a[r];
    int i = l - 1, j = r;
    for (;;) {
        while (a[++i] < p) {} // i 1й элемент с начала > p
        while (a[--j] > p) if (j == l) break;  // j 1й элемент с конца < p
        if (i >= j) break;
        swap(a[i], a[j]);
    }
    swap(a[i], a[r]);
    return i;
}

// Сортрует массив a с индекса l по индекс r
void quickSort( int* a, int l, int r)
{
    if (r - l < 1) return;
    int i = partition( a, l, r );
    quickSort(a, l, i - 1);
    quickSort(a, i + 1, r);
}

void quickSort( int* a, int n)
{
    quickSort(a, 0, n - 1);
}

// Перераспределяет массив так, что k-й наименьший элемент оказался в индексе k массива.
// Возвращает k-й наименьший элемент массива.
int select( int* a, int n, int k)
{
    int l = 0;
    int r = n - 1;
    while (r > l) {
        int i = partition(a, l, r);
        if (i >= k) {
            r = i - 1;
        } else {
            l = i + 1;
        }
    }
    return a[k];
}