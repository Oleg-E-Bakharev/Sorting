//
//  MSD.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/10/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef MSD_hpp
#define MSD_hpp

#include <iostream>
#include <vector>
#include "sparseVector.hpp"

// MSD (most significant digit first) сортрровка элементов одного размера. на выходе массив указателей на элементы.
// Для сортировки массива элементов см. CountingSort.
// Dist - класс распределения элементов типа T по dist.size значениям от 0 до dist.size() - 1.
// O(N+M): N=data.size(), M=key.size.
template<typename Iterator, typename Digitizer>
auto msdSort(Iterator begin, Iterator end, const Digitizer& digitizer) -> std::vector<typename std::remove_pointer<typename std::iterator_traits<Iterator>::value_type>::type*> {
    // На входе могут быть либо итераторы на элемент либо итераторы на указатель на элемент. Надо убрать "указатель", если есть.
    using ValueType = typename std::remove_pointer<typename std::iterator_traits<Iterator>::value_type>::type;
    using Ptr = ValueType*;
    using DigitType = typename Digitizer::DigitType;
    
    size_t step = digitizer.step();
	size_t sz = end - begin;
    std::vector<Ptr> result(sz);
    
    if (end - begin < 1) {
        return result;
    }
    
    // Вспомогательная структура, для единообразного итерирования либо по контейнеру элементов, либо по контейнеру указателей на элементы.
    struct {
        const Digitizer& digitizer;
        Ptr ptr(Ptr& p) { return p; }
        Ptr ptr(ValueType& r) { return &r; }
        DigitType digit(Ptr& p) { return digitizer(*p); }
        DigitType digit(ValueType& r) { return digitizer(r); }
    } convert { digitizer };
    
//    size_t base[digitizer.size + 3]; // Вспомогательный массив счётчиков.
//	size_t* offset = base + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
//    size_t* counter = base + 2; // Массив счётчиков, сдвинутый относительно offsets на 2.
	std::vector<size_t> base(digitizer.size + 2); // Вспомогательный массив счётчиков.
	auto offset = base.begin() + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
	auto counter = base.begin() + 2; // Массив счётчиков, сдвинутый относительно offsets на 2.

    for (size_t i = 0; i < digitizer.size + 1; ++i) offset[i] = 0;
    for (Iterator it = begin; it != end; ++it) counter[convert.digit(*it)]++; // conter[i] содержит количество элементов, равных i
    for (size_t i = 1; i < digitizer.size - 1; ++i) counter[i] += counter[i-1]; // counter[i] (кроме последнего) содержит количество элементов, <= i
        
		for (Iterator it = begin; it != end; ++it) {
			result[offset[convert.digit(*it)]++] = convert.ptr(*it);
		}
	
//	std::cout << "\npointers:\n";
//	for( auto& s : result) {
//		std::cout << *s << "\n";
//	}
	
	if (step < digitizer.width() - 1) {
		// Рекурсивное продвижение MSD по интервалам с одинаковым значением разряда.
		for (size_t i = 1; i < digitizer.size + 1; ++i) {
			if (base[i] - base[i-1] > 1) {
				digitizer.setStep(step + 1);
				auto nextBegin = result.begin() + base[i-1];
				auto n = nextBegin + 1;
				auto nextEnd = result.begin() + base[i];
				std::vector<Ptr> partialSorted = msdSort(nextBegin, nextEnd, digitizer);
				for (Ptr ptr : partialSorted) *(nextBegin++) = ptr;
			}
		}
	}
    
    return result;
}

void testMSD();

#endif /* MSD_hpp */
