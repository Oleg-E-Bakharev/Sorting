//
//  countingSort.h
//  Sorting
//
//  Created by Oleg Bakharev on 09/11/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef countingSort_h
#define countingSort_h

#include <iostream>
#include <vector>

// Распределительный подсчет (сортировка подсчетом).
// Key - класс распределения элементов типа T по key.size значениям от 0 до key.size() - 1.
// O(N+M): N=data.size(), M=key.size.
template<typename T, typename Dist> std::vector<T*> distribute(std::vector<T>& data, const Dist& dist) {
	using namespace std;
	size_t offset[dist.size + 1]; // Вспомогательный массив счётчиков.
	size_t* counter = offset + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
	for (size_t i = 0; i < dist.size + 1; i++) offset[i] = 0;
	for (size_t j = 0; j < data.size(); j++) counter[dist(data[j])]++; // conter[i] содержит количество элементов, равных i
	for (size_t i = 1; i < dist.size - 1; i++) counter[i] += counter[i-1]; // counter[i] (кроме последнего) содержит количество элементов, <= i

	vector<T*> result(data.size());
	for (size_t j = 0; j < data.size(); j++) result[offset[dist(data[j])]++] = &data[j];

	return result;
}

// Массив a - откуда берем. Массив b - куда кладем. n - сколько; min-max диапазон значений элеентов. C - вспомогательный массив размера max - min + 1.
//void countingSort(int* a, int* b, int n, int min, int max, int* c);

void testCountingSort();

#endif /* countingSort_h */
