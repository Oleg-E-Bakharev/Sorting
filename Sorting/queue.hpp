//
//  queue.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 26/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef queue_hpp
#define queue_hpp

#include <stdio.h>

class Queue {
    int* q;
    int n;
    int head;
    int tail;
public:
    Queue(int n): q(0), n(n), head(0), tail(0) {
        q = new int[n];
        for (int i = 0; i < n; i++) {q[i] = 0;}
    }
    ~Queue() {delete [] q;}
    
    bool empty() {return head % n == tail;}
    
    void putTail(int i) {
        q[tail++] = i;
        tail %= n;
    }
    
    int getHead() {
        head %= n;
        return q[head++];
    }

    void putHead(int i) {
        head %= n;
        head =  head ? head - 1 : n - 1;
        q[head] = i;
    }
    
    int getTail() {
        tail = tail ? tail - 1 : n - 1;
        return q[tail];
    }
};

void testQueue();

#endif /* queue_hpp */
