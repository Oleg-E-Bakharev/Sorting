//
//  Header.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 23.10.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include <iostream>
#include <vector>

// Распределительный подсчет (сортировка подсчетом).
// Key - класс распределения элементов типа T по key.size значениям от 0 до key.size() - 1.
// O(N+M): N=data.size(), M=key.size.
template<typename T, typename Dist>
std::vector<T*> distribute2(std::vector<T>& data, const Dist& dist) {
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

// Распределительный подсчет (сортировка подсчетом) на входе массив указателей на элементы.
// Для сортировки массива элементов см. CountingSort.
// Dist - класс распределения элементов типа T по dist.size значениям от 0 до dist.size() - 1.
// O(N+M): N=data.size(), M=key.size.
template<typename T, typename Dist>
std::vector<T*> distributePtr2(std::vector<T*>& data, const Dist& dist) {
	using namespace std;
	size_t offset[dist.size + 1]; // Вспомогательный массив счётчиков.
	size_t* counter = offset + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
	for (size_t i = 0; i < dist.size + 1; i++) offset[i] = 0;
	for (size_t j = 0; j < data.size(); j++) counter[dist(*data[j])]++; // conter[i] содержит количество элементов, равных i
	for (size_t i = 1; i < dist.size - 1; i++) counter[i] += counter[i-1]; // counter[i] (кроме последнего) содержит количество элементов, <= i
	
	vector<T*> result(data.size());
	for (size_t j = 0; j < data.size(); j++) result[offset[dist(*data[j])]++] = data[j];
	
	return result;
}

// LSD сортровка данных. К dist (от distribution) добавляется метод width - количество шагов LSD алгоритма, и setStep - номер шага LSD алгоритма.
template<typename T, typename Dist>
std::vector<T*> lsdSort2(std::vector<T>& data, const Dist& dist) {
	size_t step = dist.width() - 1;
	dist.setStep(step--);
	std::vector<T*> result = distribute2(data, dist);
	while(step != -1) {
		dist.setStep(step--);
		result = distributePtr2(result, dist);
	}
	return result;
}

#endif /* Header_h */
