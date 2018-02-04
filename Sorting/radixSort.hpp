//
//  radixSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 16/11/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef radixSort_hpp
#define radixSort_hpp

#include <stdio.h>

// Сортирует только положительные числа!
void radixSort(int* a, int* b, int n, unsigned* count);

void testRadix(int *a, int n);

#endif /* radixSort_hpp */
