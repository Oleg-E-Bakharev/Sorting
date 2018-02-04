//
//  MergeSort.h
//  MergeSort
//
//  Created by Oleg Bakharev on 07/08/15.
//  Copyright (c) 2015 Oleg Bakharev. All rights reserved.
//

#ifndef __MergeSort__MergeSort__
#define __MergeSort__MergeSort__

#include <limits>

/// Cливает 2 последовательно располагающихся отсортированнгых подмассива. end в расчете не участвует.
template <typename Array, typename sizeType = typename Array::size_type>
void mergeSub(Array& a, sizeType start, sizeType middle, sizeType end)
{
    typedef typename Array::value_type T;

    auto nLeft = middle - start;
    auto nRight = end - middle;
    
    // Временные массивы с частями исходного.
    Array left(nLeft + 1);
    Array right(nRight + 1);
    for (auto i = start; i < middle ; i++) {
        left[i - start] = a[i];
    }
    for (auto i = middle; i < end ; i++) {
        right[i - middle] = a[i];
    }
    
    // Добавляем в конец временных массивов сигнальный ограничитель.
    left[nLeft] = (std::numeric_limits<T>::max());
    right[nRight] = (std::numeric_limits<T>::max());
    
    auto i = 0;
    auto j = 0;
    for (auto k = start; k < end; k++) {
        a[k] = (left[i] < right[j]) ? left[i++] : right[j++];
    }
}

/// Основная рекурсивная процедура (метод "разделяй и властвуй").
template <typename Array, typename sizeType = typename Array::size_type>
void sortSub (Array& a, sizeType start, sizeType end)
{
    // Разделяем.
    auto middle = (start + end) / 2;
    if (start < middle) {
        // Рекурсивно сортируем левую половину.
        sortSub(a, start, middle);
        // Рекурсивно сортируем правую половину.
        sortSub(a, middle, end);
        // Объединяем результат.
        mergeSub(a, start, middle, end);
    }
}

template <typename Array>
void mergeSort(Array& a)
{
    sortSub(a, (size_t)0, a.size());
}

#endif /* defined(__MergeSort__MergeSort__) */
