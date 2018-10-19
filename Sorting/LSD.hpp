//
//  LSD.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 15.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef LSD_hpp
#define LSD_hpp

#include <iostream>
#include <vector>
// Распределительный поразрядный подсчет (сортировка подсчетом) по разрядам. на выходе массив указателей на элементы.
// Для сортировки массива элементов см. CountingSort.
// Dist - класс распределения элементов типа T по dist.size значениям от 0 до dist.size() - 1.
// O(N+M): N=data.size(), M=key.size.
template<typename Iterator, typename Digitizer>
auto distributeOnDigit(Iterator begin, Iterator end, const Digitizer& digitizer) -> std::vector<typename std::remove_pointer<typename std::iterator_traits<Iterator>::value_type>::type*> {
    // На входе могут быть либо итераторы на элемент либо итераторы на указатель на элемент. Надо убрать "указатель", если есть.
    using ValueType = typename std::remove_pointer<typename std::iterator_traits<Iterator>::value_type>::type;
    using Ptr = ValueType*;
    using DigitType = typename Digitizer::DigitType;
    
    // Вспомогательная структура, для единообразного итерирования либо по контейнеру элементов, либо по контейнеру указателей на элементы.
    struct {
        const Digitizer& digitizer;
        Ptr ptr(Ptr& p) { return p; }
        Ptr ptr(ValueType& r) { return &r; }
        DigitType digit(Ptr& p) { return digitizer(*p); }
        DigitType digit(ValueType& r) { return digitizer(r); }
    } convert { digitizer };

//    size_t offset[digitizer.size + 1]; // Вспомогательный массив счётчиков.
//    size_t* counter = offset + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
    std::vector<size_t> offset(digitizer.size + 1, 0); // Вспомогательный массив счётчиков.
    auto counter = offset.begin() + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.

//    for (size_t i = 0; i < digitizer.size + 1; ++i) offset[i] = 0;
    for (Iterator it = begin; it != end; ++it) counter[convert.digit(*it)]++; // conter[i] содержит количество элементов, равных i
    for (size_t i = 1; i < digitizer.size - 1; ++i) counter[i] += counter[i-1]; // counter[i] (кроме последнего) содержит количество элементов, <= i
        
    std::vector<Ptr> result(end - begin);
		for (Iterator it = begin; it != end; ++it)  {
			result[offset[convert.digit(*it)]++] = convert.ptr(*it);
		}
    
    return result;
}

// LSD (least significant digit first) сортровка данных одинакового размера. К dist (от distribution) добавляется метод width - количество шагов LSD алгоритма, и setStep - номер шага LSD алгоритма.
template<typename Iterator, typename Dist>
auto lsdSort(Iterator begin, Iterator end, const Dist& dist) -> std::vector<typename std::iterator_traits<Iterator>::pointer> {
    // Получаем указатель на элемент, на котороый указывает итератор.
    using Ptr = typename std::iterator_traits<Iterator>::pointer;
	size_t step = dist.width() - 1;
	dist.setStep(step--);
	std::vector<Ptr> result = distributeOnDigit(begin, end, dist);
	while(step != -1) {
		dist.setStep(step--);
        result = distributeOnDigit(result.begin(), result.end(), dist);
	}
	return result;
}

void testLSD();

#endif /* LSD_hpp */
