//
//  matrix.h
//  Sorting
//
//  Created by Oleg Bakharev on 30/03/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include <iostream>
#include <iomanip>
#include <valarray>
#include <vector>
#include <numeric>
#include <cassert>
#include <initializer_list>
#include "for_iter.h"

using namespace std;

// А-ля Страуструп: http://www.stroustrup.com/matrix.c
template<typename T> class slice_iter {
	typedef vector<T> VT;
	VT& v;
	slice s;
    // Вместо T& используем typename vector<T>::reference для совместимости с vector<bool>
    typename VT::reference ref(size_t i) const { return (v)[s.start() + i * s.stride()]; }
public:
    typedef T value_type;
    typedef typename VT::reference reference;
    typedef typename VT::const_reference const_reference;
    slice_iter( VT& v, slice s ) : v(v), s(s) {}

    // Заменитель конструктора для константных экземпляров. Обычный конструктор "возвратил бы" не const итератор.
    static const slice_iter ct(const VT& v, slice s) { return slice_iter( const_cast<VT&>(v), s ); }
	
	size_t size() const { return s.size(); }
	const_reference operator[](size_t i) const { return ref(i); }
	reference operator[](size_t i) { return ref(i); }
	
    // Перемножение соотв. элементов со сложением результатов.
	T operator*(const slice_iter& si) const {
		assert(size() == si.size());
		T res = 0;
		for( size_t i = 0, s = size(); i < s; i++ ) {
			res += ref(i) * si[i];
		}
		return res;
	}
	
	// Для for(:)
	for_iter_t<slice_iter> begin() { return for_iter(*this); }
	for_iter_t<slice_iter> end() { return for_iter(*this); }
	for_iter_t<const slice_iter> begin() const { return for_iter(*this); }
	for_iter_t<const slice_iter> end() const { return for_iter(*this); }
};

template <typename T>
class matrix {
    size_t _w;
    size_t _h;
    vector<T> _m;
    
public:
    using vec = slice_iter<T>;
    using value_type = vec;
    using reference = vec; // vec это и value_type и reference.
    using const_reference = const vec;
    
    matrix(size_t w, size_t h) : _w(w), _h(h), _m(w * h) {}
	
	matrix(initializer_list<initializer_list<T>> l) {
        _h = l.size();
        _w = _h > 0 ? l.begin()->size() : 0;
		_m.resize( _w * _h );
		size_t pos = 0;
		for( initializer_list<T> const& rowList : l ) {
			assert(rowList.size() == _w);
			for( const T& value : rowList) {
				_m[pos] = value;
				pos++;
			}
		}
	}
	
	size_t w() const { return _w; }
    size_t h() const { return _h; }
    
    typename vec::reference operator () (size_t x, size_t y) { return _m[ _w * y + x ]; }
	
    vec col(size_t x) { return vec( _m, slice(x, _h, _w) ); }
    const vec col(size_t x) const { return vec::ct( _m, slice(x, _h, _w) ); }
	
    vec row(size_t y) { return vec( _m, slice( y * _w, _w, 1) ); }
    const vec row(size_t y) const { return vec::ct( _m, slice( y * _w, _w, 1) ); }
	
	vec operator[] (size_t y) { return row(y); }
	const vec operator[] (size_t y) const { return row(y); }
	
    matrix transpond() const {
        auto m = matrix(_h, _w);
        for ( int i = 0; i < _h; i++ ) {
            int j = 0; j < _w; j++;
            m(j, i) = *this(i, j);
        }
        return m;
    }
	
    matrix operator*( const matrix& m ) const {
        assert( m.h() == _w );
        auto res = matrix(m.w(), _h);
        for( size_t y = 0, h = res.h(); y < h; y++ ) {
            for( size_t x = 0, w = res.w(); x < w; x++ ) {
				res(x, y) = row(y) * m.col(x);
            }
        }
		return res;
    }
    
    size_t size() const { return _h; }
	
	// Для for(:)
	for_iter_t<matrix> begin() { return for_iter(*this); }
	for_iter_t<matrix> end() { return for_iter(*this); }
	for_iter_t<const matrix> begin() const { return for_iter(*this); }
	for_iter_t<const matrix> end() const { return for_iter(*this); }
};

template <typename T>
std::ostream& operator << (std::ostream& os, const matrix<T>& m) {
    using namespace std;
    for( auto row : m ) {
        for (auto x : row ) {
            cout << setw(2) << x << ", ";
        }
        cout << endl;
    }
    cout << "\n\n";
    return os;
}

inline void testMatrix() {
	
	auto m1 = matrix<int> {
		{1, 2, 3},
		{2, 2, 3}
	};
	cout << m1;
	
	auto m2 = matrix<int> {
		{2, 3},
		{3, 3},
		{4, 4}
	};
	cout << m2;
	
	cout << m1 * m2;
    cout << m2 * m1;
    
    const matrix<int>& m3 = m1;
    
    for( auto row : m3 ) {
        for ( auto& x : row ) {
            x = 1;
        }
        cout << endl;
    }
    
    cout << m3;
    
    matrix<bool> mb = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    
    cout << mb;
    
    mb[0][0] = 1;
    
    cout <<mb;
    
    bool b = mb[1][1];
    cout << b;

    for( auto row : mb ) {
        for ( auto x : row ) {
            x = 1;
        }
        cout << endl;
    }

    cout << mb;
}

#endif /* matrix_h */
