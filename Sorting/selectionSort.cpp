//
//  selectionSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 12/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "selectionSort.hpp"

void selectionSort(int* a, int count)
{
    for (int i = 0; i < count; i++) {
        int best = i;
        for (int j = i + 1; j < count; j++) {
            if (a[j] < a[best]) {
                best = j;
            }
        }
        std::swap(a[i], a[best]);
    }
}