//
//  queue.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 26/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "queue.hpp"
#include "assert.h"

void testQueue() {
    auto queue = Queue(10);
    queue.putHead(-1);
    assert(queue.getHead() == -1);
    assert(queue.empty());
    queue.putHead(-1);
    assert(queue.getHead() == -1);
    assert(queue.empty());
    queue.putTail(1);
    assert(queue.getTail() == 1);
    assert(queue.empty());
    queue.putTail(1);
    assert(queue.getTail() == 1);
    assert(queue.empty());
    queue.putTail(1);
    assert(queue.getHead());
    assert(queue.empty());
    queue.putHead(1);
    assert(queue.getTail() == 1);
    assert(queue.empty());
}
