//
//  quickSort.h
//  Sorting
//
//  Created by Oleg Bakharev on 25/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef quickSort_h
#define quickSort_h

#include <stdio.h>

// Сортрует массив
//void quickSort( int* a, int n);

// Разбивает массив так, что k-й наименьший элемент оказался в индексе к массива.
// Возвращает k-й наименьший элемент.
int select( int* a, int n, int k);

#endif /* quickSort_h */
