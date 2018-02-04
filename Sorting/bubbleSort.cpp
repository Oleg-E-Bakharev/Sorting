//
//  bubbleSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 12/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "bubbleSort.hpp"

void bubbleSort(int* a, int count)
{
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if (a[j+1] < a[j]) {
                std::swap(a[j+1], a[j]);
            }
        }
    }
}