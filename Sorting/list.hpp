//
//  list.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 26/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef list_hpp
#define list_hpp

#include <stdio.h>
#include <assert.h>
#include <new>
#include <iostream>

template <class T>
struct ListNode {
    T key;
    ListNode* next;
    ListNode* prev;
    
    ListNode(T key = 0) : key(key), next(nullptr), prev(nullptr) {}
    
    bool isFree() { return next == nullptr && prev == nullptr;}
    
    void swap(ListNode* node) {
        assert(node != this);
        assert(node != nullptr);
        auto tPrev = prev;
        auto tNext = next;
        auto nPrev = node->prev;
        auto nNext = node->next;
        
        if(tPrev == node)tPrev = this;
        if(tNext == node)tNext = this;
        if(nPrev == this)nPrev = node;
        if(nNext == this)nNext = node;
        
        prev = nPrev;
        next = nNext;
        node->prev = tPrev;
        node->next = tNext;
        if(tPrev) tPrev->next = node;
        if(tNext) tNext->prev = node;
        if(nPrev) nPrev->next = this;
        if(nNext) nNext->prev = this;
    }
};

template <typename T>
class List {
    ListNode<T> nil;
public:
    List() : nil(0) {nil.prev = &nil; nil.next = &nil;}
    
    ListNode<T>& head() { return *nil.next; }
    ListNode<T>& tail() { return *nil.prev; }
    int size() { return nil.key; }
    bool isHead(ListNode<T>& node) {return node.prev == &nil;}
    bool isTail(ListNode<T>& node) {return node.next == &nil;}

    ListNode<T>* search(int key) {
        for (ListNode<T>* node = &head(); node != &nil ; node = node->next) {
            if (node->key == key) {
                return node;
            }
        }
        return nullptr;
    }
     
    void insert(ListNode<T>& node, ListNode<T>& pos) {
        assert(node.isFree());
        auto next = pos.next;
        pos.next = &node;
        node.prev = &pos;
        node.next = next;
        next->prev = &node;
        assert(nil.key >= 0);
        nil.key++;
    }
    
    ListNode<T>& remove(ListNode<T>& node) {
        assert(&node != &nil);
        node.prev->next = node.next;
        node.next->prev = node.prev;
        node.next = node.prev = nullptr;
        nil.key--;
        assert(nil.key >= 0);
        return node;
    }
    
    void sort(ListNode<T>& from, ListNode<T>& to);
    void sort();
};

template <typename T>
std::ostream& operator << (std::ostream& out, List<T>& list);

template <typename T>
class ListAllocator {
    int n;
    ListNode<T>* free;
    ListNode<T>* nodes;
public:
    ListAllocator(int n) : n(n + 1) {
        // Последний элемент - индикаторный.
        n += 1;
        nodes = new ListNode<T>[n];
        free = nodes;
        for (int i = 0; i < n - 1; i++) {
            nodes[i].next = &nodes[i + 1];
        }
        nodes[n-1].next = nullptr;
    }
    
    ~ListAllocator() { delete [] nodes; }
    
    int capacity() { return n - 1; }
    
    ListNode<T>* alloc(T i) {
        ListNode<T>* node = free;
        if (free->next == nullptr) {
            return nullptr;
        }
        free = node->next;
        new (node) ListNode<T>(i);
        return node;
    }
    
    void dealloc(ListNode<T>& node) {
        node.~ListNode();
        node.next = free;
        free = &node;
    }
};

void testList();
void testListSort(int N);

template <typename T>
ListNode<T>* mergeNodes(ListNode<T>* a, ListNode<T>* b)
{
    ListNode<T> head(0);
    head.next = a;
    ListNode<T>* c = &head;
    while (a != 0 && b != 0) {
        //        cout << a->key << " : " << b->key << endl;
        if(a->key < b->key) {
            c->next = a; c = a; a = a->next;
        } else {
            c->next = b; c = b; b = b->next;
        }
    }
    c->next = (a == 0) ? b : a;
    
    return head.next;
}

template <typename T>
ListNode<T>* sortList(ListNode<T>* c)
{
    if (c == 0 || c->next == 0) {
        return c;
    }
    ListNode<T>* a = c;
    ListNode<T>* b = c->next;
    while ((b != 0) && (b->next != 0)) {
        c = c->next;
        b = b->next->next;
    }
    b = c->next;
    c->next = 0;
    return mergeNodes(sortList(a), sortList(b));
}

template <typename T>
void List<T>::sort()
{
    if (nil.key < 2) {
        return;
    }
    tail().next = 0;
    ListNode<T>* node = sortList(&head());
    ListNode<T>* prev = &nil;
    nil.next = node;
    for (; node->next != 0; prev = node, node = node->next) {
        node->prev = prev;
    }
    nil.prev = node;
    node->next = &nil;
}

#endif /* list_hpp */
