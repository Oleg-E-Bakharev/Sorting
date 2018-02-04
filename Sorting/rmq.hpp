//
//  rmq.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 09/01/17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef rmq_hpp
#define rmq_hpp

#include <vector>
#include <cmath>
#include <cassert>

// Range Minimum Query
// Вычисляет минимум на отрезке массива за O(1) при препроцессинге за O(n*log(n))
template<typename T, typename V = std::vector<T>>
class RMQ {
    std::vector<std::vector<T>> _st; // Sparce Table. Разреженная таблица ST[k][i] есть минимум на полуинтервале [A[i], A[i+2^k])
    std::vector<size_t> _log2; // логарифмы степеней двойки.
    
    // Препроцессинг. Заполняем _st. O(n*log(n))
    void prepare_(const V& v) {
        using namespace std;
        size_t vSize = v.size();
        prepareLog2_(vSize + 1);
        size_t stSize = _log2[vSize] + 1;
        _st.reserve(stSize);
        
        // Первую строку заполняем из массива v.
        _st.push_back(vector<T>(vSize));
        vector<T>& row = _st.back();
        for (size_t j = 0; j < vSize; j++) {
            row[j] = v[j];
        }
        
        // Остальные строки заполняем как min(ST[k-1][i], ST[k-1][i + 2^(k — 1)])
        for (size_t i = 1; i < stSize; i++) {
            const vector<T>& prev = _st.back();
            const size_t step = (1 << (i - 1)); // 2^(k — 1).
            size_t rowSize = prev.size() - step;
            _st.push_back(vector<T>(rowSize));
            vector<T>& row = _st.back();
            for (size_t j = 0; j < rowSize; j++) {
                row[j] = min(prev[j], prev[j + step]);
            }
        }
    }
    
    // Заполняет _log2 на размер size.
    void prepareLog2_(size_t size) {
        _log2.reserve(size);
        _log2.push_back(0);
        for (size_t i = 1; i <= size; i++) {
            _log2.push_back(log2(i));
        }
    }
	
public:
    RMQ(const V& v) {
        prepare_(v);
    }
    
    // Запрос минимума на отрезке. O(1)
    T query(size_t begin, size_t end) const {
        using namespace std;
        assert(begin < end && end < _log2.size());
        // Учитывая, что любой отрезок массива разбивается на два перекрывающихся подотрезка длиною в степень двойки.
        const size_t width = _log2[end - begin + 1]; // индекс строки в _st - целочисл. логарифм2 ширины запрашиваемого отрезка.
        const size_t endPos = end - (1 << width) + 1; // левая границы полуинтервала длинной (end - 2^width)
        const T rmq = min(_st[width][begin], _st[width][endPos]);
        return rmq;
    }
};

template<typename V>
RMQ<typename V::value_type, V> makeRmq( const V& v) { return RMQ<typename V::value_type, V>(v); }

void testRMQ();

#endif /* rmq_hpp */
