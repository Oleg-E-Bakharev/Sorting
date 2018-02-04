//
//  Common.h
//  Common
//
//  Created by Oleg Bakharev on 05/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef Common_h
#define Common_h

#include <iostream>
#include <vector>
#include <valarray>
#include <array>
#include <chrono>

template<typename Array>
typename std::enable_if<
std::is_same<Array, std::vector<typename Array::value_type> >::value
|| std::is_same<Array, std::valarray<typename Array::value_type> >::value
|| std::is_same<Array, std::array<typename Array::value_type, sizeof(Array) / sizeof(typename Array::value_type) > >::value
, std::ostream &>::type
operator<< (std::ostream& out, const Array& v) {
    out << "[";
    auto space = "";
    for (auto i = 0; i < v.size(); i++) {
        std::cout << space;
        std::cout << v[i];
        space = " ";
    }
    out << "]" << std::endl;
    return out;
}

template <class Container>
Container monotonicSequence(size_t size, typename Container::value_type start = 0, typename Container::value_type step = 1)
{
    Container container;
    
    for (size_t i = 0; i < size ; i++) {
        container.push_back(start + step * (typename Container::value_type)i);
    }
    
    return container;
}

// Измеритель времени функции.
template<typename T>
size_t measureTime(const T& func) {
	using namespace std::chrono;
	auto start = high_resolution_clock::now();
	func();
	return duration_cast<microseconds>(high_resolution_clock::now() - start).count();
}

// Измеритель времени сортировки с перемешиванием входного массива.
template<typename V, typename R, typename T>
size_t measureSort(V& v, R& randomEngine, const T& sort) {
	std::shuffle(v.begin(), v.end(), randomEngine);
	size_t time = measureTime([&v, sort]{ sort(); });
	if(!std::is_sorted(v.begin(), v.end())) {
		std::cout << "Bad! Bad!\n";
	}
	
	return time;
}

#endif /* Common_h */
