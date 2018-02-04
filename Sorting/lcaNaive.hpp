//
//  lcaNaive.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 11/01/17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef lcaNaive_hpp
#define lcaNaive_hpp

#include <stdio.h>
#include <vector>

// Наивный алгоритм вычисления наибольшего общего предка двух вершин дерева.
// p - вектор индексов предков вершин. У корня предок -1.
// Память O(n), Время O(h).
template<typename T = size_t, typename V = std::vector<T>>
size_t lcaNaive1(const V& p, T i, T j) {
    using namespace std;
    vector<size_t> search; // вектор обхода.
    search.assign(p.size(), 0);
    // Поднимаемся параллельно по узлам помечая пройденные пока не встретим уже посещенный. На нем останавливаемся.
    while(i != -1 && j != -1) {
        if(i != -1) {
            if (search[i] == 1) {
                return i;
            }
            search[i] = 1;
            i = p[i];
        }
        if(j != -1) {
            if (search[j] == 1) {
                return j;
            }
            search[j] = 1;
            j = p[j];
        }
    }
    return -1;
};

// Наивный алгоритм вычисления наибольшего общего предка двух вершин дерева.
// p - вектор индексов предков вершин. У корня предок -1.
// Память O(1), Время O(h).
template<typename T = size_t, typename V = std::vector<T>>
T lcaNaive2(const V& p, T i, T j) {
    using namespace std;
    
    struct depth {
        static T calc(const V& p, T i) {
            T depth = 0;
            while (p[i] != -1) {
                depth++;
                i = p[i];
            }
            return depth;
        }
    };
    
    T h1 = depth::calc(p, i);
    T h2 = depth::calc(p, j);
    
    // Понятно, что общий узел будет при h1 == h2. Поэтому для начала выравниваем h1 и h2.
    if (h2 > h1) {
        swap(h1, h2);
        swap(i ,j);
    }
    while(h1 != h2) {
        i = p[i];
        h1--;
    }
    // А теперь синхронно поднимаемся пока не попадем в общий узел.
    while (i != j) {
        i = p[i];
        j = p[j];
    }

    return -1;
};

void testLcaNaive();

#endif /* lcaNaive_hpp */
