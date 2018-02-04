//
//  timSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 11/10/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef timSort_hpp
#define timSort_hpp

#include <iostream>
#include <vector>
#include <algorithm> // reverse, upper_bound, copy
#include <iterator> // reverse_iterator
#include <assert.h>

using namespace std;

// Timsort. Гибридная стабильная сортировка O(n*log(n)) худш. сл. O(n*log(n)) сред. сл. O(n) луч. сл.
// Очень эффективна для сортировки частично-упорядоченных последовательностей.
// http://bugs.python.org/file4451/timsort.txt
// https://habrahabr.ru/post/251751/
// https://www.youtube.com/watch?v=NVIjHj-lrT4

template <typename T> class TimSort_T {
    T* _begin; // начало.
    T* _end; // элемент, следующий за последним.
    size_t _minrun; // Минимальный размер монотонной цепочки.
    std::vector<T*> _runs; // Стек цепочек.
    std::vector<T> _mergeTemp; // Временный массив слияния цепочек.
    
    /// Сортировка вставками.
    void insertionSort( T* begin, T* end )
    {
        for ( T* i = begin + 1; i !=  end; i++ ) {
            // Перед каждой итерацией a[0..i-1] содержит уже отсортированный массив.
            T key = *i;
            T* j = i;
            // Ищем место вставки a[i] в отсортированную последовательность а[0..i].
            while ( j != begin && *(j-1) > key ) {
                *(j) = *(j-1);
                j--;
            }
            *(j) = key;
        }
    }
    
    // Определение minrun.
    size_t calcMinrun() {
        size_t n = _end - _begin;
        size_t r = 0;
        while (n >= 64) {
            r |= n & 1;
            n >>= 1;
        }
        return n + r;
//        return 10;
    }
    
    //  Поиск неубывающей цепочки размера не менее minRun.
    T* findRun(T* pos) {
		assert(pos <= _end);
        T* begin = pos;
        bool isNotDecreasing = *pos <= *(pos + 1);
        while (pos != _end) {
            ++pos;
            // условие окончание монотонной цепочки.
            if( *(pos - 1) <= *(pos) != isNotDecreasing ) {
                break;
            }
        }
        assert(pos > begin && pos <= _end);
        bool shouldSort = false;
        bool trimmedFromEnd = false;
        
        // Если до конца массива остается меньше чем _minrun, то оставляем в конце размер _minrun.
		if (_end - pos < _minrun) {
            pos = _end - _minrun;
            trimmedFromEnd = true;
        }
        
        // Если длина цепочки < _minrun то дополняем цепочку до _minRun (или до конца в случае trimFromEnd).
        if ( pos < begin || pos - begin < _minrun  ) {
            if (trimmedFromEnd) {
                pos = _end;
            } else {
                pos = begin + _minrun;
            }
			assert(pos <= _end);
            shouldSort = true;
        }
        
        if (shouldSort) {
            insertionSort(begin, pos);
        } else if (!isNotDecreasing) {
            std::reverse(begin, pos);
        }
		assert(pos > begin);
		assert(pos <= _end);
        return pos;
    }
    
    // Построение массива неубывающих цепочек _runs.
    void buildRuns() {
        _runs.push_back(_begin);
        do {
            _runs.push_back(findRun(_runs.back()));
			runsCollapse_();
        } while(_runs.back() != _end);
    }
	
	size_t runLen(size_t n) const {
		return _runs[n] - _runs[n-1];
	}
    
    // Слияние при наличии трех и более цепочек.
    void runsCollapse_() {
		while (_runs.size() > 2) {
			// Пытаемся уменьшить количество цепочек, одновременно обеспечивая убывание их размеров и соотнешение их
			// размеров а-ля Фибоначчи, напр. 128 64 32 16 8 4 2 2.
			size_t n = _runs.size() - 2;
			if (n > 2 && runLen(n-1) <= runLen(n) + runLen(n+1) ) { // X > Y + Z;
				if (runLen(n-1) <= runLen(n+1)) {
					T* posLast = _runs.back(); _runs.pop_back();
					mergeLastRuns();
					_runs.push_back(posLast);
				}
				mergeLastRuns();
			} else if (runLen(n) <= runLen(n+1) ) { // Y > Z;
				mergeLastRuns();
			} else {
//				cout << "runs: ";
//				for( int i = 1; i < _runs.size(); i++ ) {
//					cout << runLen(i) << ", ";
//				}
//				cout << endl;
				break; // Инвариант установлен!
			}
		}
    }
	
	// Слияние двух последних цепочек.
	void mergeLastRuns() {
		size_t n = _runs.size() - 2;
		T* posZ = _runs[n+1];
		T* posY = _runs[n];
		T* posX = _runs[n-1];
		assert(posZ >= posY && posY >= posX);
		mergeCloseRuns(posX, posY, posZ);
		_runs.pop_back();
		_runs.back() = posZ;
	}
	
    // Слияние двух цепочек.
    void mergeCloseRuns(T* x, T* y, T* z) {
		assert(z >= y && y >= x);
//        for( T* p = x; p != z; p++ ) { cout << *p << ", "; }; cout << endl;
        // Для обеспечения стабильности сливаем только соседние цепочки и только в том порядке в каком они идут.
        // Помещаем минимальную цепочку в _mergeTemp и если минимальная цепочка справа то сливаем их справа налево,
        // иначе сливаем цепочки слева направо.
        size_t sizeXY = y - x;
        size_t sizeYZ = z - y;
        if (sizeXY > sizeYZ) {
			_mergeTemp.assign(y, z);
			T* yTemp = &_mergeTemp.front();
			T* zTemp = (&_mergeTemp.back()) + 1;
            using ReversePointer = std::reverse_iterator<T*>;
			mergeRunsProc(ReversePointer(z),
                           ReversePointer(y),
                           ReversePointer(x),
						   ReversePointer(zTemp),
						   ReversePointer(yTemp),
						   std::greater<T>() // оболочка оператора > для обеспечения стабильности сортировки.
						   );
        } else {
			_mergeTemp.assign(x, y);
			T* xTemp = &_mergeTemp.front();
			T* yTemp = (&_mergeTemp.back()) + 1;
			// less_equal - оболочка оператора <= для обеспечения стабильности сортировки.
			mergeRunsProc(x, y, z, xTemp, yTemp, std::less_equal<T>());
        }
    }

//    template<typename Iter, typename Comp>
//    // Процедура сравнивает два элемента, возвращает минимальный и продвигает соответствующий указатель вперед.
//	// Дополнительно считаем последовательные продвижения того или иного итератора.
//    T getMinMoveNext(Iter& first, Iter& second, size_t& cntIncFirst, size_t& cntIncSecond, Comp comp) {
//		if (comp(*first, *second)) {
//			++cntIncFirst;
//			cntIncSecond = 0;
//			return *(first++);
//		}
//		++cntIncSecond;
//		cntIncFirst = 0;
//		return *(second++);
//    }
	
    // Процедура слияния соседних цепочек. Предполагается, что размер цепочки yz, не меньше размера цепочки xy.
	// x ,y, z - итераторы границ смежных сливаемых подмассивов. xTemp, yTemp - итераторы копии подмассива xy.
	template<typename Iter, typename Comp>
    void mergeRunsProc(Iter x, Iter y, Iter z, Iter xTemp, Iter yTemp, Comp comp) {
//		if (std::distance(x, z) < 1000) {
//			std::merge(xTemp, yTemp, y, z, x, comp);
//			return;
//		}
//        for( auto p = x; p != z; p++ ) { cout << *p << ", "; }; cout << endl;
//        for( auto p = xTemp; p != yTemp; p++ ) { cout << *p << ", "; }; cout << endl;
//		Iter pos1(xTemp);
//        Iter pos2(y);
        
//		uint_fast64_t cntInc1 = 0;
//		uint_fast64_t cntInc2 = 0;
		
		assert(yTemp - xTemp >= 0);
		assert(z - y >= 0);
		
        while (xTemp != yTemp) {
			assert(yTemp - xTemp >= 0);
			assert(z - y >= 0);
			//cout << *(x-1) << ", ";
			if (y == z) {
				// Копируем остаток temp в конец xz.
				std::copy(xTemp, yTemp, x);
				break;
			}
			// Проверки на включение галлопа.
//			if (cntInc1 == 7) {
////				cout << "gallop on cnt1\n";
//				xTemp = gallopMerge(xTemp, yTemp, x, *y, comp);
//				cntInc1 = 0;
//			} else if (cntInc2 == 7) {
////				cout << "gallop on cnt2\n";
//				y = gallopMerge(y, z, x, *xTemp, comp);
//				cntInc2 = 0;
//			} else {
				if (comp(*xTemp, *y)) {
//					cntInc1++;
//					cntInc2 = 0;
					*(x++) = *(xTemp++);
				} else {
//					cntInc2++;
//					cntInc1 = 0;
					*(x++) = *(y++);
				}
//			}
			
//			*(x++) = getMinMoveNext(pos1, pos2, cntInc1, cntInc2, comp);
        }
//		cout << endl;
    }
	
	// Режим слияния галлопом.
    template<typename Iter, typename Comp>
	Iter gallopMerge(Iter srcBegin, Iter srcEnd, Iter& dst, T barier, Comp comp) {
		// Бинарным поиском в src последовательности ищем позицию первого элемента большего barier.
		// После чего копируем все найденные элементы в src.
		// static_assert ставим чтобы гарантировать что upper_bound будет работать в режиме бинарного поиска.
		static_assert(
					  std::is_same<std::random_access_iterator_tag,
					  typename std::iterator_traits<Iter>::iterator_category>::value,
					  "The gallopMerge() function only accepts random access iterators or raw pointers to an array.\n");
		
		Iter firstGreaterPos = std::upper_bound(srcBegin, srcEnd, barier, comp);
		std::copy(srcBegin, firstGreaterPos, dst);
		dst += firstGreaterPos - srcBegin;
		return firstGreaterPos;
	}
	
    void mergeRuns() {
        // Сливаем все соседние цепочки, начиная с конца (с самых коротких).
        while (_runs.size() > 2) {
			T* runsBack = _runs.back();
            mergeCloseRuns(_runs[_runs.size()-3], _runs[_runs.size()-2], runsBack);
			_runs.pop_back();
			_runs.back() = runsBack;
//            for( T* p = _begin; p != _end; p++ ) { cout << *p << ", "; }; cout << endl;
        }
    }
    
public:
    TimSort_T(T* begin, T* end) : _begin(begin), _end(end), _minrun(calcMinrun()) {
        buildRuns();
//        for( T* p = _begin; p != _end; p++ ) { cout << *p << ", "; }; cout << endl;
        mergeRuns();
    }
};

template<typename T> void timSort(T* begin, T* end) { TimSort_T<T> ts(begin, end); }

void testTimSort();

#endif /* timSort_hpp */
