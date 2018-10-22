//
//  MSD2.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 21/10/2018.
//  Copyright © 2018 Oleg Bakharev. All rights reserved.
//

#ifndef MSD2_h
#define MSD2_h

#include <vector>

// D::size - размер словаря, D:width - количество разрядов, D::CounterType тип счётчика разрядов.
template <typename It, typename D> class MSDSorter {
    using value_type = typename std::iterator_traits<It>::value_type;
    using CounterType = std::vector<typename D::DigitType>;
    std::vector<value_type> _temp;
    std::vector<CounterType> _counters;
    size_t _pos = 0;
    size_t _width;
    
    inline void msdStep_(It begin, It end) {
        _counters.emplace_back(CounterType(D::DigitRange + 1, 0));
        auto counter = _counters.back();
        _temp.assign(begin, end);
        auto offset = counter.begin() + 1; // counter смещенный на 1
        for (auto i = begin; i != end; ++i) ++offset[D::digitIndex(*i, _pos)];
        // conter[i] содержит количество элементов, равных i
        for (auto j = counter.begin() + 2, prev = j - 1; j != counter.end() - 1; ++j, ++prev) *j += *prev;
        // counter[i] (кроме последнего) содержит количество элементов, <= i
        for (size_t i = 0; i < end - begin; ++i) *(begin + counter[D::digitIndex(_temp[i], _pos)]++) = std::move(_temp[i]);
        // По всем интервалам одинакового значения текущего разряда производим сортировку по следующему розряду.
        if (_pos == _width - 1) {
            return;
        }
        _pos++;
        for (auto j = 1; j != counter.size() - 1; j++) {
            size_t next = counter[j];
            size_t curr = counter[j-1];
            assert(next >= curr);
            if (next - curr > 1) {
                msdStep_(begin + curr, begin + next);
            }
        }
        _pos--;
        _counters.pop_back();
    }
    
public:
    MSDSorter(It begin, It end, size_t width) : _counters(D::DigitRange), _width(width) {
        msdStep_(begin, end);
    }
};

template<typename It, typename Digitizer>
void msdSort2(It begin, It end, const Digitizer& digit) {
    MSDSorter<It, Digitizer> msd(begin, end, digit.width());
}

#endif /* MSD2_h */
