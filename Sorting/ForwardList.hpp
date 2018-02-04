//
//  ForwardList.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 10.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef ForwardList_hpp
#define ForwardList_hpp

#include <iostream>
#include "assert.h"

template <typename T> class ForwardList {
	struct Node {
		T key;
		Node* next;
		
		Node(T t, Node* next = nullptr) : key(t), next(next) {}
	};
	
	using Link = Node*;
    
    Link _head = nullptr;
    Link _tail = nullptr;
    
    Link& addAt_(Link& pos, T t) {
        return pos = new Node(t, pos ? pos : nullptr);
    }
    
    T delAt_(Link& pos) {
        Link node = pos;
        T t = node->key;
        pos = pos->next;
        delete node;
        fixTail_(pos);
        return t;
    }
    
    Link fixTail_(Link& pos) {
        if (!pos || !pos->next) {
            _tail = pos;
        }
        return pos;
    }
    
    void reverse_(Link pos, Link prev) {
        if(pos == nullptr) {
            return;
        }
        Link next  = pos->next;
        pos->next = prev;
        reverse_(next, pos);
    }
    
    // Из двух узлов списка, возвращает минимальный, продвигая соотв. параметр вперед.
    Link moveMin_(Link& a, Link& b)
    {
        Link& min = a->key <= b->key ? a : b;
        Link result = min;
        min = min->next;
        return result;
    }
    
    // Объединяет два отсортированных списка в один.
    Link mergeNodes_(Link a, Link b)
    {
        Link head = moveMin_(a, b);
        Link c = head;
        while (a && b) {
            c->next = moveMin_(a, b);
            c = c->next;
        }
        c->next = a ? a : b;
        
        return head;
    }
    
    // Сортирует список с узла c.
    Link sort_(Link c)
    {
        // Разрываем список посередине и сортируем каждую половину отдельно, затем сливаем.
        if (!c || !c->next) {
            return c;
        }
        Link a = c;
        Link b = c->next;
        while (b && b->next) {
            c = c->next;
            b = b->next->next;
        }
        b = c->next;
        c->next = nullptr;
        return mergeNodes_(sort_(a), sort_(b));
    }
    
public:
    bool isEmpty() { return _head == _tail; }
    
    void addTail(T t) {
        Link& pos = _tail ? _tail->next : _head;
        _tail = addAt_(pos, t);
    }

    void addHead(T t) {
        Link& pos = _head ? _head : _tail;
        _head = addAt_(pos, t);
    }
    
    T delHead() {
        assert(_head);
        return delAt_(_head);
    }
    
	bool search(T t) {
        for (Link n = _head; n; n = n->next) {
			if (n->key == t) {
				return true;
			}
		}
		return false;
	}
    
    void reverse() {
        reverse_(_head, nullptr);
		std::swap(_tail, _head);
    }
    
    // Сортировка слиянием.
    void sort() {
        _head = sort_(_head);
        while( _tail->next ) {
            _tail = _tail->next;
        }
    }
	
    ~ForwardList() {
        while (_head) {
            delAt_(_head);
        }
        assert(_head == nullptr && _tail == nullptr);
    }
    
    friend std::ostream& operator<<(std::ostream& os, ForwardList& fl) {
        os << "{";
        for (Link node = fl._head; node != nullptr; node = node->next ) {
            os << node->key << ", ";
        }
        os <<"}\n";
        return os;
    }
};

void testForwardList();

#endif /* ForwardList_hpp */
