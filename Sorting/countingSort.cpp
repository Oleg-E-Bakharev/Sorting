//
//  countingSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 09/11/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "countingSort.h"
#include <iostream>
#include "testSort.hpp"
#include <vector>
#include "rnd.h"
#include <chrono>
#include <string>

using namespace std;
using namespace chrono;

struct Student {
	std::string name; // Имя
	size_t phys; // Результат по физике. 1..4
	size_t tech; // Результат по технике. 1..4
	
	friend std::ostream& operator<<(std::ostream& os, const Student& s) {
		return os << s.name << " " << s.phys << ", " << s.tech << "\n";
	}
};

struct PhysDist {
	const size_t size = 4;
	size_t operator()(const Student& s) const { return s.phys - 1; }
};

struct TechDist {
	const size_t size = 4;
	size_t operator()(const Student& s) const { return s.tech - 1; }
};

// Массив a - откуда берем. Массив b - куда кладем. n - сколько; min-max диапазон значений элеентов. C - вспомогательный массив размера max - min + 1.
//void countingSort(int* a, int* b, int n, int min, int max, int* c)
//{   int i{0}, j{0}, m{max - min};
//    for (i = 0; i < m; i++) c[i] = 0;
//    for (j = 0; j < n; j++) c[a[j] - min + 1]++; // Ci содержит количество элементов, равных i
//    for (i = 1; i < m; i++) c[i] += c[i-1]; // Сi содержит количество элементов, <= i
//    for (j = 0; j < n; j++) b[(c[a[j] - min])++] = a[j];
//}

void testCountingSort()
{
	vector<Student> students{
		{"Anderson", 2, 3},
		{"Brown", 3, 1},
		{"Davis", 3, 2},
		{"Garcia", 4, 2},
		{"Harris", 1, 4},
		{"Jackson", 3, 4},
		{"Johnson", 4, 3},
		{"Jones", 3, 3},
		{"Martin", 1, 3},
		{"Martinez", 2, 2},
		{"Miller", 2, 1},
		{"Moore", 1, 3},
		{"Robinson", 2, 3},
		{"Smith", 4, 4},
		{"Taylor", 3, 2},
		{"Thomas", 4, 1},
		{"Thomson", 4, 2},
		{"White", 2, 4},
		{"Willams", 3, 3},
		{"Wilson", 4, 2}
	};
	
	for (const Student& s : students) { cout << s; };
	
	cout << "\nPhys distribution:\n";
	for (const auto* s : distribute(students, PhysDist{})) { cout << *s; }
	
	cout << "\nTech distribution:\n";
	for (const auto* s : distribute(students, TechDist{})) { cout << *s; }
//    auto a = new int[n];
//    for (int i = 0; i < n ; i++) a[i] = random(min, max);
//    int* b = new int[n];
//    int* c = new int[max - min + 1];
////    for (int i = 0; i < n; i++) {
////        cout << a[i] << ", ";
////    }
//    cout << endl;
//	auto start = high_resolution_clock::now();
//    countingSort(a, b, n, min, max, c);
//	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
//	std::cout << "Counting sort: " << performance << endl;
////    for (int i = 0; i < n; i++) {
////        cout << b[i] << ", ";
////    }
//    testSort(b, n);
//    delete [] a;
//    delete [] b;
//    delete [] c;
}