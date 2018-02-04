//
//  shellSort.h
//  Sorting
//
//  Created by Oleg Bakharev on 06/11/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef shellSort_h
#define shellSort_h

// Сортировка Шелла по Седжвику с шагами 1 4 8 13 40 121 364 1093 3280 9841

void shellSort(int* a, int n)
{
    int h = 0;
    for (h = 1; h <= n / 9; h = 3 * h + 1); // Находим максимальный h
    for (; h > 0; h /= 3) {
        for (int i = h; i < n; i++) {
            int v = a[i];
            int j = i;
            for (; j >= h && v < a[j - h]; j -= h) {
                a[j] = a[j-h];
            }
            a[j] = v;
        }
    }
}

#endif /* shellSort_h */
