//
//  quickSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 12/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef quickSort_hpp
#define quickSort_hpp

#include <functional>
#include <iostream>
#include "rnd.h"
#include "insertionSort.h"

using namespace std;

template<typename It>
inline It medianPos(It begin, It end) {
    auto mid = begin + ((end - begin) >> 1);
    if (*end < *begin) std::swap(begin, end);
    if (*mid < *begin) std::swap(begin, mid);
    if (*end < *mid) std::swap(mid, end);
    return mid;
}

// Nico Lomuto
template <typename It>
inline It lomutoPartition(It begin, It end)
{
    --end;
    std::swap(*medianPos(begin, end), *end);
    auto pivot = *end;
    
    for (It cur = begin; end - cur >= 0; ++cur) {
        if (*cur < pivot) {
            std::swap(*begin, *cur);
            ++begin;
        }
    }
    std::swap(*begin, *end);
    return begin;
}

// Charles Hoare 1950
template <typename It>
inline It hoarePartition(It begin, It end) {
    
    It median = medianPos(begin, end - 1);
    std::swap(*begin, *median);
    auto pivot = *begin;
    median = begin;
    
    while (true) {
		while (*--end > pivot); // Слева цепочка элементов ограничена опорным.
		while (*++begin < pivot && end - begin > 0){}; // Справа необходимо явное условие.
        if (end - begin <= 0) break;
        std::swap(*begin, *end);
    }
    std::swap(*median, *end);
    return end;
}

// 3-way partition возвращает a[lo..hi) == pivot
template <typename It>
inline std::pair<It, It> fatPartition(It begin, It end)
{
    It median = medianPos(begin, end - 1);
    std::swap(*begin, *median);
    auto pivot = *begin;
    It lo = begin, hi = end;
    for (It i = begin + 1 ; hi - i > 0;) {
        if (*i < pivot) {
          std::swap(*(lo++), *(i++));
        }
        else if (*i > pivot) {
            std::swap(*i, *(--hi)); }
        else i++;
    } // теперь a[begin..lo] < pivot = a[lo..hi) < a[hi, end)
    return {lo, hi - 1};
}

template <typename It>
void quickSort(It begin, It end) {
    if (end - begin > 1) {
        It mid = hoarePartition(begin, end);

		quickSort(begin, mid);
        quickSort(mid + 1, end);
    }
}

// Перераспределяет массив так, что k-й наименьший элемент оказался в индексе k массива.
// Возвращает k-й наименьший элемент массива.
template<typename It, typename T>
int select( It begin, It end, T k)
{
	It array = begin;
	while (end - begin > 1) {
		It mid = partition(begin, end);
		if (mid - array >= k) {
			end = mid;
		} else {
			begin = mid + 1;
		}
	}
	return array[k];
}

// Bentley & McIlroy 1993
template <typename It>
inline std::pair<It,It> fatBMPartition(It begin, It end) {
    using std::swap;
    It first = medianPos(begin, end - 1);
    swap(*begin, *first); // стаивм медиану первым элементом.
    auto pivot = *begin;
    first = begin;

    It left = begin; // левая позиция элемениов равных опорному.
    It right = end; // правая позиция элементов равных опорному.
    It last = right;

    while (true) {
        while (*--end > pivot); // Слева цепочка элементов ограничена опорным.
        while (*++begin < pivot && end - begin > 0); // Справа необходимо явное условие.
        if (end - begin <= 0) break;
        swap(*begin, *end);
		
        // Все элементы равные опорному группируем на концах отрезка.
        if(*begin == pivot) {
            swap(*++left, *begin);
        }
        if(*end == pivot) {
            swap(*--right, *end);
        }
    }
    std::swap(*first, *end); // Ставим опорный элемент в позицию встречи begin и end.
	begin = end; // Если нет повторяющихся опорных элементов, то возвратим позицию опорного элемента.
    //Перемещаем элементы равные опорному с краёв в центр.
    while (first != left) swap(*first++, *--end);
    while (last != right) swap(*--last, *++begin);
    return {end, begin};
}


template <typename It>
inline void sort3(It begin, It mid, It end) {
    if (*end < *begin) std::swap(*begin, *end);
    if (*mid < *begin)  std::swap(*begin, *mid);
    if (*end < *mid) std::swap(*mid, *end);
}

//template <typename It>
//void fatSort(It begin, It end) {
//
//    size_t difference = end - begin;
//    switch (difference) {
//        case 0:
//        case 1:
//            return;
//
//        case 2:
//            if (*--end < *begin) std::swap(*begin, *end);
//            return;
//
//        case 3:
//            sort3(begin, begin + 1, --end);
//            return;
//
//        default:
//            break;
//    }
//
//    if (difference < 30) {
//        insertionSort(begin, end);
//        return;
//    }
//
//    pair<It,It> mid = fatPartition(begin, end);
//
//    fatSort(begin, mid.first);
//    fatSort(mid.second + 1, end);
//}

template <typename It, std::pair<It, It> partFunc(It, It) = fatBMPartition>
void fastSort(It begin, It end) {
    
    size_t difference = end - begin;
    switch (difference) {
        case 0:
        case 1:
            return;
			
        case 2:
            if (*--end < *begin) std::swap(*begin, *end);
            return;
			
        case 3:
            sort3(begin, begin + 1, --end);
            return;
			
        default:
            break;
    }
    
    if (difference < 30) {
        insertionSort(begin, end);
        return;
    }
	
    pair<It,It> mid = partFunc(begin, end);
    
    fastSort(begin, mid.first);
    fastSort(mid.second + 1, end);
}

template <typename Container>
size_t hoarePartition2(Container& a, size_t p, size_t r)
{
    auto x = a[p];
    size_t i = p;
    size_t j = r - 1;
    while (i < j) {
        while (a[i] >= x && i < r - 1) {
            i++;
        }
        while (a[j] <= x && j > p) {
            j--;
        }
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
    
    std::swap(a[p], a[j]);
    
    return j;
}

template <typename Container>
size_t lomutoPartition3(Container& a, size_t p, size_t r)
{
    auto pivot = a[r-1];
    size_t i = p;
    for (size_t j = p; j < r - 1; ++j) {
        if (a[j] < pivot) {
            std::swap(a[i], a[j]);
            ++i;
        }
    }
    std::swap(a[i], a[r-1]);
	
    return i;
}

template <typename Container>
size_t randomizedPartition3(Container& a, size_t p, size_t r)
{
    auto pivot = (size_t)random((int)p, (int)r);//a[r-1];
    size_t i = p;
    for (size_t j = p; j < r - 1; j++) {
        if (a[j] < pivot) {
            std::swap(a[i], a[j]);
            i++;
        }
    }
    std::swap(a[i], a[r-1]);
    
    return i;
}


template <typename Container, typename Partition>
void quickSortStep(Container& a, size_t p, size_t r, Partition partition)
{
    if (p < r) {
        size_t q = partition(a, p, r);
        quickSortStep(a, p, q, partition);
        quickSortStep(a, q+1, r, partition);
    }
}

template <typename Container, typename Partition = decltype(lomutoPartition3<Container>)>
void quickSort(Container& a, Partition partition = lomutoPartition3)
{
    quickSortStep(a, 0, a.size(), partition);
}

//template <typename Container, typename Partition = typeof lastPivotPartition<Container>>
//void measureQuickSort(Container& array, const char* message, Partition partition = lastPivotPartition) {
//    startPerformanceCounter();
//    quickSort(array, partition);
//    double performance = getPerformanceCounter();
//    std::cout << message << performance << std::endl;
//}

template <typename Container>
void randomizedSelect(Container& a, size_t p, size_t r, typename Container::value_type i )
{
    if (p == r) {
        return a[p];
    }
    size_t q = randomizedPartition(a, p, r);
    size_t k = q - p;
    if( i == k ) {
        return a[q];
    } else if( i < k) {
        return randomizedSelect(a, p, q - 1);
    } else {
        return randomizedSelect(a, q + 1, r, i - k);
    }
}

#endif /* quickSort_hpp */

