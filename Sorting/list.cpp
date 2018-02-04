//
//  list.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 26/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#include "list.hpp"
#include <assert.h>
#include "rnd.h"
#include "testSort.hpp"
#include <chrono>

using namespace std;
using namespace std::chrono;

template <typename T>
void randomize(List<T>& l)
{
    ListNode<T>** nodes = new ListNode<T>*[l.size()];
    ListNode<T>* node = &l.head();
    for (int i = 0; i < l.size(); i++) {
        nodes[i] = node;
        node = node->next;
    }
    
    int count = l.size();
    for (int i = 0; i < count - 1; i++) {
        int j = random(i + 1, count);
        assert(j > i);
        nodes[i]->swap(nodes[j]);
    }
    delete [] nodes;
}

template <typename T>
void testListConsistency(List<T>& list )
{
    ListNode<T>* node = &list.head();
    ListNode<T>* nil = node->prev;
    int i = 0;
    for (;node != list.tail().next; node = node->next) {
        assert(node->prev->next = node);
        assert(node->next->prev = node);
        if (list.size() != 1) {
            assert(node->next->next != node);
            assert(node->prev->prev != node);
        }
        i++;
    }
    assert(list.isHead(list.head()));
    assert(list.isTail(list.tail()));
    assert(i == list.size());
    assert(node == nil);
}

void testList()
{
    ListAllocator<int> la(100);

    
    auto l1 = List<int>();

    testListConsistency(l1);
    for (int i = 0; i < la.capacity(); i++) {
        l1.insert(*la.alloc(i), l1.tail());
    }
    cout << "original:\n" << l1 << endl;
    testListConsistency(l1);
    
    auto node = &l1.head();
    for (int i = 0; i < l1.size() - 1; i++) {
        node->swap(node->next);
    }
    cout << "swapped head:\n" << l1 << endl;
    testListConsistency(l1);
    
    node = &l1.tail();
    for (int i = 0; i < l1.size() - 1; i++) {
        node->swap(node->prev);
    }
    cout << "swapped tail:\n" << l1 << endl;
    testListConsistency(l1);
    
    randomize(l1);
    cout << "randomized:\n" << l1 << endl;
    testListConsistency(l1);
    
    l1.sort();
    cout << "sorted:\n" << l1 << endl;
    testListConsistency(l1);
    
    for (int i = 0; i < la.capacity() / 2; i++) {
        
        la.dealloc(l1.remove(l1.tail()));
    }
    cout << "delete first half:\n" << l1 << endl;
    testListConsistency(l1);
    
    auto l2 = List<int>();
    for (int i = 0; i < la.capacity() / 2; i++) {
        l2.insert(*la.alloc(i), *l2.head().prev);
    }
    cout << "inser first half:\n" << l2 << endl;
    testListConsistency(l2);

}

void testListSort(int N)
{
    ListAllocator<int> la(N);
    
    auto l = List<int>();
    
    for (int i = 0; i < la.capacity(); i++) {
        l.insert(*la.alloc(i), l.tail());
    }
    testListConsistency(l);
    
    randomize(l);
	auto start = high_resolution_clock::now();
    l.sort();
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
	std::cout << "list mergesort performance: " << performance << endl;
    
    testListConsistency(l);
	
    auto A = new int[N];
    auto node = &l.head();
    for (int i = 0; i < l.size(); i++) {
        A[i] = node->key;
        node = node->next;
    }
    
    testSort(A, N);
}

template <typename T>
std::ostream& operator << (std::ostream& out, List<T>& list) {
    char c = '{';
    for (ListNode<T>* node = &list.head(); node != list.tail().next; node = node->next) {
        out << c << " " << node->key;
        c = ',';
    }
    out << " }" << std::endl;
    return out;
}
