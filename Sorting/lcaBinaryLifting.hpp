//
//  lcaBinaryLifting.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 12/01/17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef lcaBinaryLifting_hpp
#define lcaBinaryLifting_hpp

#include <vector>
#include <assert.h>
#include <iostream>
#include "common.h"

// Решение задачи наименьшего общего предка методом двоичного подъема.
// Препроцессинг за O(n*log(n)). Запрос за O(log(n)).
// https://neerc.ifmo.ru/wiki/index.php?title=%D0%9C%D0%B5%D1%82%D0%BE%D0%B4_%D0%B4%D0%B2%D0%BE%D0%B8%D1%87%D0%BD%D0%BE%D0%B3%D0%BE_%D0%BF%D0%BE%D0%B4%D1%8A%D0%B5%D0%BC%D0%B0
template<typename T = size_t, typename V = std::vector<T>>
class LcaBinaryLifting {
    
    class Matrix {
        std::vector<T> _data;
        T _rows;
        T _cols;
    public:
        Matrix(T rows, T cols) : _data(rows * cols, 0), _rows(rows), _cols(cols) {}
        
        size_t rows() const { return _rows; }
        size_t cols() const { return _cols; }
        
        T& operator()(T row, T col) { return _data[row * _cols + col]; }
        const T& operator()(T row, T col) const { return _data[row * _cols + col]; }
        
        T* row(T row) { return &_data[row * _cols]; }
        const T* row(T row) const { return &_data[row * _cols]; }
    };
    
    V _p; // Вектор предков узлов. Размер n. У корня _p[r] == r.
    T root; // Корень дерева.
    std::vector<T> _d; // Вектор глубин узлов.
    
    // dp[i][v] — номер вершины, в которую мы придем если пройдем из вершины v вверх по подвешенному дереву 2^i шагов,
    // причем если мы пришли в корень, то мы там и останемся.
    // Размер n х пол log(n)).
    Matrix _dp;
    
    // Препроцессинг. O(n*log(n));
    void preprocess_() {
        fillDepth();
        
        fillDepthLog();
    }
    
    // Вычисление вектора глубин дерева _d. За счет динамики O(n).
    void fillDepth() {
        for (T i = 0; i < _p.size(); i++) {
            if(_d[i] == -1) {
                _d[i] = depth_(i);
            }
        }
    }
    
    // Вычисление глубины узла. O(n).
    T depth_(T i) {
        T p = _p[i];
        if(p == -1) {
            p = i;
            _p[i] = i;
        }
        if (p == i) {
            return 0;
        }
        _d[p] = depth_(p);
        return _d[p] + 1;
    }
    
    // Заполнение _dp. O(n*log(n)).
    void fillDepthLog() {
        // Заполняем первый уровень.
        T* row = _dp.row(0);
        for (T j = 0; j < _dp.cols(); j++) {
            row[j] = _p[j];
        }
        for (T i = 1; i < _dp.rows(); i++) {
            row = _dp.row(i);
            for(T j = 0; j < _dp.cols(); j++) {
                row[j] = _dp(i-1, _dp(i-1, j));
            }
        }
    }
    
    // Вернет l: 2^l <= n ; 2^(l+1) > n
    size_t log_(T n) {
        size_t l = 1;
        do {
            l++;
        } while ((1 << l) <= n); // 2^l
        return l - 1;
    }
    
public:
    // Инициализация вектором предков. Размер n.  У корня _p[r] == -1. O(n*log(n))
    LcaBinaryLifting(V p) : _p(p), root(-1), _d(p.size(), -1), _dp(log_(p.size()), p.size()) {
        preprocess_();
        
        for( auto i = 0; i < p.size(); i++) std::cout << i << " ";
        std::cout << "\nparent:\n" << _p;
        std::cout << "\ndepth:\n" << _d;
        std::cout << "\n\nDepthLog:\n";
        T* row;
        for (T i = 0; i < _dp.rows(); i++) {
            row = _dp.row(i);
            for(T j = 0; j < _dp.cols(); j++) {
                std::cout << _dp(i,j) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n\n";
    }
    
    // Запрос LCA за O(log(n)).
    size_t query(T u, T v) {
        std::cout << "LCA "<< u << ", " << v << ":\n";
        // Для корректной работы _d[u] должно быть <= _d[v].
        if (_d[u] > _d[v]) {
            std::swap(u, v);
        }
        
        std::cout << "make equal depths\n";
        // Выравниваем высоты узлов для нахождения LCA. O(log(n))
        // Рассматриваем двоичное предсталение разницы d[v]-d[u]
        // и поднимаемся на i-й ненулевой разряд.
        auto depthDelta = _d[v] - _d[u];
        for (auto i = _dp.rows() - 1; i != -1; i--) {
            if (depthDelta >= (1 << i)) { // 2^i
                std::cout << "moving " << v << " to " << _dp(i,v) << "\n";
                v = _dp(i,v);
                depthDelta = _d[v] - _d[u];
            }
        }
        std::cout << "result depth: " << _d[u] << "\n";
        assert(_d[u] == _d[v]);
        
        if (u == v) {
            std::cout << _dp(0, u) << "\n\n";
            return u;
        }
        
        std::cout << "make binary lifting\n";
        // Двоичным подъемом находим общего предка. O(log(n))
        for (T i = _dp.rows() - 1; i != -1; i--) {
            auto du = _dp(i, u);
            auto dv = _dp(i, v);
            if ( du != dv) {
                std::cout << "moving " << u << " to " << du << "\n";
                std::cout << "moving " << v << " to " << dv << "\n";
                u = du;
                v = dv;
            }
        }
        assert(_dp(0, u) == _dp(0, v));
        std::cout << _dp(0, u) << "\n\n";
        return _dp(0, u);
    }
};

template<typename V>
LcaBinaryLifting<typename V::value_type, V> makeLcaBL( const V& v) { return LcaBinaryLifting<typename V::value_type, V>(v); }

void testLcaBinaryLifting();

#endif /* lcaBinaryLifting_hpp */
