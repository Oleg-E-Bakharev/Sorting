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
It medianPos(It begin, It end) {
    auto mid = begin + ((end - begin) >> 1);
    if (*end < *begin) std::swap(begin, end);
    if (*mid < *begin) std::swap(begin, mid);
    if (*end < *mid) std::swap(mid, end);
    return mid;
}

template <typename It>
It lomutoPartition(It begin, It end)
{
    --end;
    std::swap(*medianPos(begin, end), *end);
    auto pivot = *end;
    
//        cout << "pivot: " << pivot << "\n";
    
    for (It cur = begin; end - cur >= 0; ++cur) {
        if (*cur < pivot) {
            std::swap(*begin, *cur);
            ++begin;
        }
    }
    std::swap(*begin, *end);
    return begin;
}


template <typename It>
It hoarePartition(It begin, It end) {
    
    It median = medianPos(begin, end - 1);
    std::swap(*begin, *median);
    auto pivot = *begin;
    median = begin;
    
//    cout << "pivot: " << pivot << "\n";
    
    while (true) {
		while (*--end > pivot);
		while (*++begin < pivot && end - begin > 0);
        if (end - begin <= 0) break;
        std::swap(*begin, *end);
    }
    std::swap(*median, *end);
    return end;
}

template <typename It>
void quickSort(It begin, It end) {
    if (end - begin > 1) {
        
//                cout << "\nstep\n";
//                auto first = *begin;
//                auto last = *(end-1);
//                cout << "begin: " << first << "\n";
//                cout << "end: " << last << "\n";
        
        It mid = hoarePartition(begin, end);
//        It mid = lomutoPartition(begin, end);

//        cout << "med: " << *mid << "\n";
        
//        for (auto i = begin; i != end; i++) cout << *i << ", ";
        
        quickSort(begin, mid);
        quickSort(mid + 1, end);
        
//                cout << "\n";
    }
}


// Bentley & McIlroy 1993
template <typename It>
std::pair<It,It> fatPartition(It begin, It end) {
    using std::swap;
    It first = medianPos(begin, end - 1);
    swap(*begin, *first); // стаивм медиану первым элементом.
    auto pivot = *begin;
    first = begin;

//													cout << "pivot: " << pivot << "\n";
	
    It left = begin; // левая позиция элемениов равных пивоту.
    It right = end; // правая позиция элементов равных пивоту.
    It last = right;

    while (true) {
//													for (auto i = first; i != last; ++i) cout << *i << ", "; cout << "\n";
//													for (auto i = begin; i != end; ++i) cout << *i << ", "; cout << "\n";
        while (*--end > pivot); // Слева цепочка всегда ограничена пивотом.
		while (*++begin < pivot && end - begin > 0); // Справа необходимо явное условие.

//													for (auto i = begin; i != end + 1; ++i) cout << *i << ", "; cout << "\n";
        if (end - begin <= 0) break;
        swap(*begin, *end);
        // Все элементы равные пивоту группируем на концах отрезка.
//													for (auto i = left; i != right + 1; ++i) cout << *i << ", "; cout << "\n";
        if(*begin == pivot) {
            swap(*++left, *begin);
        }
        if(*end == pivot) {
            swap(*--right, *end);
        }
//													for (auto i = left; i != right + 1; ++i) cout << *i << ", "; cout << "\n";
    }
//													for (auto i = first; i != last; i++) cout << *i << ", "; cout << "\n";
//													It f = first;
//													It l = last;
//	
//    assert(*end != pivot);
    std::swap(*first, *end); // Ставим пивот в точку встречи begin и end.
	begin = end; // Если нет повторяющихся пивотов, то вернем позицию пивота.
//													for (auto i = first; i != last; i++) cout << *i << ", "; cout << "\n";
//	end--;
//    end = begin - 1;
//    ++begin;
    
//													for (auto i = end; i != begin + 1; ++i) cout << *i << ", "; cout << "\n";
	
    while (first != left) swap(*first++, *--end);
    while (last != right) swap(*--last, *++begin);
    //Перемещаем элементы равные пивоту с краёв в центр.

//													for (auto i = f; i != l; i++) cout << *i << ", "; cout << "\n";
//													for (auto i = end-1; i != begin + 2; ++i) cout << *i << ", "; cout << "\n";

    return {end, begin};

//    --end;
//    auto pivot = *medianPos(begin, end);
//
//    while (true) {
//        while (*begin < pivot) ++begin;
//        It frontEqual = begin;
//        while (*begin == pivot) ++begin;
//        while (*end > pivot) --end;
//        It backEqual = end;
//        while (*end == pivot) --end;
//        if (end - begin >= 0) std::swap(*begin, *end);
//        else return {frontEqual, backEqual};
//    }
//
//    return {begin, end};
}


template <typename It> void sort3(It begin, It mid, It end) {
    if (*end < *begin) std::swap(*begin, *end);
    if (*mid < *begin)  std::swap(*begin, *mid);
    if (*end < *mid) std::swap(*mid, *end);
}

template <typename It>
void optimizedSort(It begin, It end) {
    
    size_t difference = end - begin;
    switch (difference) {
        case 0:
        case 1:
            return;
        case 2:
            if (*--end < *begin) {
                std::swap(*begin, *end);
            }
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
	
//        cout << "\nstep\n";
//        auto first = *begin;
//        auto last = *(end-1);
//        cout << "begin: " << first << "\n";
//        cout << "end: " << last << "\n";

//        It mid = hoarePartition(begin, end);
//        It mid = lomutoPartition(begin, end);
//        optimizedSort(begin, mid);
//        optimizedSort(mid + 1, end);
    pair<It,It> mid = fatPartition(begin, end);
    
//    cout << "partiotion: " << *mid.first <<", "<<*mid.second << "\n";
//    for (auto i = begin; i != end; i++) cout << *i << ", "; cout << "\n";

//    pair<It,It> mid = lomutoPartitionOpt(begin, end);
    optimizedSort(begin, mid.first);
    optimizedSort(mid.second + 1, end);
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

template <typename Container, typename Partition = typeof lomutoPartition3<Container>>
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

