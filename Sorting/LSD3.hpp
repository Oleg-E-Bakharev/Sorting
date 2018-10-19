//
//  LSD3.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 11/10/2018.
//  Copyright © 2018 Oleg Bakharev. All rights reserved.
//

#ifndef LSD3_h
#define LSD3_h

#include <iostream>
#include <vector>

// Dist item(at, pos).
template<typename It, typename Digitizer>
class LSDSorter {
    using value_type = typename std::iterator_traits<It>::value_type;
    
    const Digitizer& _digit;
    
    std::vector<value_type> _temp;
    std::vector<size_t> _counter;
    
    It _begin;
    It _end;
    It _resBegin;
    It _resEnd;
    
    void lsdStep_() {
        _counter.assign(_counter.size(), 0);
        auto offset = _counter.begin() + 1; // _counter смещенный на 1
        for (auto i = _begin; i != _end; ++i) ++offset[_digit(*i)];
        // conter[i] содержит количество элементов, равных i
        for (auto j = _counter.begin() + 2, prev = j - 1; j != _counter.end() - 1; ++j, ++prev) *j += *prev;
        // counter[i] (кроме последнего) содержит количество элементов, <= i
        for (auto i = _begin; i != _end; ++i) *(_resBegin + _counter[_digit(*i)]++) = std::move(*i);
    }
    
public:
    LSDSorter(It begin, It end, const Digitizer& digit) : _digit(digit), _counter(_digit.size + 1), _begin(begin),
    _end(end), _temp(end - begin), _resBegin(_temp.begin()), _resEnd(_temp.end())  {
        for (size_t step = _digit.width() - 1; step != -1; --step) {
            _digit.setStep(step);
            lsdStep_();
            std::swap(_begin, _resBegin);
            std::swap(_end, _resEnd);
        }
        if (_digit.width() % 2) {
            // число шагов нечетное надо скопировать _temp в исходный массиы.
            for (It i = _temp.begin(); i != _temp.end(); ++i) {
                *(_begin++) = *i;
            }
        }
    }
};

template<typename It, typename Digitizer>
void lsdSort3(It begin, It end, const Digitizer& digit) {
    LSDSorter<It, Digitizer> msd(begin, end, digit);
}

void testLSD3();

#endif /* LSD3_h */

