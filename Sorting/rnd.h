//
//  rnd.h
//  test1
//
//  Created by Oleg Bakharev on 06/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef rnd_h
#define rnd_h

#include <stdio.h>
#include <random>

void randomInit();

int random( int i, int j );

template <class Container>
void randomize(Container& a)
{
    shuffle(a.begin(), a.end(), std::default_random_engine{std::random_device{}()});

//    for (int i = 0; i < a.size(); i++) {
//        int j = random(i, (int)a.size());
//        auto tmp = a[i];
//        a[i] = a[j];
//        a[j] = tmp;
//    }
}

template <typename T>
void randomize(T* begin, T* end) {
	for (T* i = begin; i != end; i++) {
		T j = random(i, end - begin);
		T tmp = *(begin+i);
		*(begin+i) = *(begin+j);
		*(begin+j) = tmp;
	}
}

template <class Container>
Container randomSequence(size_t size, int min, int max)
{
    Container array;
    for (size_t i=0; i < size ; i++) {
        array[i] = random(min, max);
    }
    
    return array;
}

static inline void randomizeIntArray(int* a, int n)
{
    for (int i = 0; i < n ; i++) {
        int j = random(i, n);
        int tmp = a[i];
        a[i] = a[j];
        a[j] = tmp;
    }
}

#endif /* rnd_h */
