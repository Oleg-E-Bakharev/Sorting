//
//  MSDQSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 13.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef MSDQSort_hpp
#define MSDQSort_hpp

#include <iostream>
#include <utility>
#include <vector>
#include <assert.h>

// Dist item(at, pos).
//template<typename It, typename Digitizer>
//class MSDQSorter {
//	//	using It = typename std::vector<T>::iterator;
//	const Digitizer& _digit;
//	inline std::pair<It,It> msdPartition_(It begin, It end, size_t pos) {
//		using std::swap;
//		//		swap(*begin, *medianPos(begin, end - 1)); // стаивм медиану первым элементом.
//		auto pivot = _digit(*begin, pos);//*begin;
//
//		It left = begin; // левая позиция элементов равных опорному.
//		It head = begin;
//		It tail = end;
//		It right = end; // правая позиция элементов равных опорному.
//									for (It i = begin; i != end; ++i) { std::cout << *i <<"\n"; }; std::cout <<"*1\n";
//		while (true) {
//			while (_digit(*--tail, pos) > pivot); // Слева цепочка элементов ограничена опорным.
//			while (_digit(*++head, pos) < pivot && tail - head > 0); // Справа необходимо явное условие.
//			if (tail - head <= 0) break;
//			swap(*head, *tail);
//
//			// Все элементы равные опорному группируем на концах отрезка.
//			if(_digit(*head, pos) == pivot) {
//				swap(*++left, *head);
//			}
//			if(_digit(*tail, pos) == pivot) {
//				swap(*--right, *tail);
//			}
//		}
//									for (It i = begin; i != end; ++i) { std::cout << *i <<"\n"; };std::cout <<"*2\n";
//		//		assert(head - tail == 1);
//		if (right - left <= 2 && _digit(*(left + 1), pos) == pivot ) { // Все символы этого разряда текущего отрезка (begin-end) одинаковы.
//			return {begin, end};
//		}
//
//		//		std::swap(*first++, *tail); // Ставим опорный элемент в позицию встречи begin и end.
//		head = tail+1; // Если нет повторяющихся опорных элементов, то возвратим позицию опорного элемента.
//		// Если в позиции tail оказался опорный элемент, то оставим его на месте.
//		if (tail - begin > 1 && _digit(*tail, pos) == pivot) {
//			--tail;
//			assert(tail != begin);
//		}
//		assert(tail-begin>=0);
//
//		//							for (It i = begin; i != end; ++i) { std::cout << *i <<"\n"; }; std::cout <<"*3\n";
//
//		// Перемещаем элементы равные опорному с краёв в центр.
//
//		if (left != begin)
//			for (It first = begin; left - first >= 0; swap(*first++, *tail--));
//		if (right != end)
//			for (It last = end; last != right; swap(*--last, *head++));
//
////		for (It first = begin; left - first >= 0; swap(*first++, *tail--));
//////		for (It first = begin; left - first > 0; swap(*first++, *tail--));
////		//		while (left - first >= 0) swap(*first++, *tail--);
////		for (It last = end; last != right; swap(*--last, *head++));
////		//		while (last != right) swap(*--last, *head++);
////		//							for (It i = begin; i != end; ++i) { std::cout << *i <<"\n"; }; std::cout <<"*\n";
//		return {tail + 1, head};
//	}

// Dist item(at, pos).
template<typename It, typename Digitizer>
class MSDQSorter {
	const Digitizer& _digit;
	inline std::pair<It,It> msdPartition_(It begin, It end, size_t pos) {
		using std::swap;
		//		swap(*begin, *medianPos(begin, end - 1)); // стаивм медиану первым элементом.
		auto pivot = _digit(*begin, pos);
		for (It i = begin + 1; i != end;) {
			auto ch = _digit(*i, pos);
			if (ch < pivot)
				swap(*begin++, *i++);
			else if (ch > pivot)
				swap(*i, *--end);
			else ++i;
		}
		// begin .. end диавазон ch == pivot.
		return {begin, end};
	}


	void msdSort_(It begin, It end, size_t pos) {
		if (end - begin <= 1) return;
		
		std::pair<It,It> mid = msdPartition_(begin, end, pos);
		
		msdSort_(begin, mid.first, pos);
		if (_digit(*mid.first, pos) != 0) {
			msdSort_(mid.first, mid.second, pos + 1);
		}
		msdSort_(mid.second, end, pos);
	}
	
public:
	MSDQSorter(It begin, It end, const Digitizer& digit) : _digit(digit) {
		msdSort_(begin, end, 0);
	}
};

template<typename It, typename Digitizer>
void msdQSort(It begin, It end, const Digitizer& digit) {
	MSDQSorter<It, Digitizer> msd(begin, end, digit);
}

void testMSDQSort();

#endif /* MSDQSort_hpp */

/*
 // i-й разряд ключа t c основанием R.
 template <typename T, size_t R> struct Digit {
 size_t operator()(const T& t, size_t i) {
 return t >> (i << R) & (R-1);
 }
 };
 
 // Специяализация для строк. i-я буква ключа.
 template <size_t R> struct Digit<std::string, R> {
 size_t operator()(const std::string& s, size_t i) {
 return s[i >> R] >> (i % R) & (R-1);
 }
 };
 
 // Функция ускоритель вызова с автоматическим выводом параметров шаблона.
 template <typename T, size_t R = 8> size_t digit(const T& t, size_t i) {
 return Digit<T,R>{}(t, i);
 }
 
 // Общее решение.
 template <typename It, size_t R> class MSDSorter {
 public:
 void operator()(It begin, It end, size_t d) {}
 };
 
 // Специализация для основания 2 - binary quick sort.
 template <typename It> class MSDSorter<It, 2> {
 It partition(It begin, It end, size_t d) {
 while (end != begin) {
 while (digit(*--end) == 1 && end !=  begin);
 while (digit(*begin) == 0 && end != ++begin);
 std::swap(*begin, *end);
 }
 return end;
 }
 
 void binaryQSort(It begin, It end, size_t d) {
 using T = decltype(*begin); // Тип элемента сортировки.
 if( end - begin <= 1 || d > (sizeof(T) << 8)) return;
 It mid = partition(begin, end, d);
 binaryQSort(begin, mid + 1, d + 1);
 binaryQSort(mid, end, d + 1);
 }
 public:
 void operator()(It begin, It end, size_t d) { binaryQSort(begin, end, 0); }
 };
 
 
 // Поразрядна сортировка - ключи - это слова состоящие из букв.
 // R - radix, разрядность буквы ключа.
 template <typename It, size_t R = 8>
 void msdSort(It begin, It end) {
 using T = decltype(*begin); // Тип элемента сортировки.
 
 }
 
 #include <iostream>
 #include <vector>
 
 template<typename T, typename Dist>
 class MSDSorter {
 const Dist& _dist;
 std::vector<T*> _data;
 size_t* _offset; // Вспомогательный массив счётчиков.
 
 std::vector<T*> distribute_(size_t lo, size_t hi) {
 using namespace std;
 size_t* counter = _offset + 1; // Массив счётчиков, сдвинутый относительно offsets на 1.
 for (size_t i = 0; i < _dist.size + 1; i++) _offset[i] = 0;
 for (size_t j = 0; j < _data.size(); j++) counter[dist(_data[j])]++; // conter[i] содержит количество элементов, равных i
 for (size_t i = 1; i < _dist.size - 1; i++) counter[i] += counter[i-1]; // counter[i] (кроме последнего) содержит количество элементов, <= i
 
 vector<T*> result(_data.size());
 for (size_t j = 0; j < _data.size(); j++) result[_offset[dist(_data[j])]++] = _data[j];
 
 _data = result;
 }
 
 public:
 MSDSorter(std::vector<T>& data, const Dist& dist) : _dist(dist), _data(data.size(), _offset[dist.size + 2] {
 
 }
 std::vector<T*>& operator()() { return _result; }
 };
 
 
 // Распределительный подсчет (сортировка подсчетом).
 // Key - класс распределения элементов типа T по key.size значениям от 0 до key.size() - 1.
 // O(N+M): N=data.size(), M=key.size.
 template<typename T, typename Dist>
 std::vector<T*> MSDSort(std::vector<T>& data, const Dist& dist) {
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
 */

