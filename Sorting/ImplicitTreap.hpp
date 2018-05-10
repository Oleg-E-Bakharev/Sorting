//
//  ImplicitTreap.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 10/05/2018.
//  Copyright © 2018 Oleg Bakharev. All rights reserved.
//

#ifndef ImplicitTreap_hpp
#define ImplicitTreap_hpp

#include <iostream>
#include <iomanip>
#include <assert.h>
#include <cstdint>
#include <random>
#include "TreeInfo.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////
// Декартово дерево по неявному ключу. https://habr.com/post/102364/

template <typename T> class ImplicitTreap {
    
    // Узел.
    struct Node {
        T item;
        size_t size = 1; // Неявный ключ.
        size_t priority; // Случайный приоритет.
        Node* l = nullptr; // Левый потомок.
        Node* r = nullptr; // Правый потомок.
        bool reversed = false; // Флаг инверсиии для обращения элементов.
        
        Node(const T& item):item(item) {}
        
        friend std::ostream& operator << (std::ostream& os, const Node& it) {
            os << it.item;
            return os;
        }
    };
    
    using Link = Node*;
    
    std::default_random_engine _random {std::random_device{}()};
    
    Link _root = nullptr;
    
    size_t size_(Link n) const {
        return n ? n->size : 0;
    }
    
    void resize_(Link n) const {
        if (n) { n->size = size_(n->l) + size_(n->r) + 1; }
    }
    
    // Порядковая статистика.
    Link searchAt_(size_t pos) const {
        assert(pos < size_(_root));
        Link n = _root;
        while(n) {
            if (size_(n->l) == pos) break;
            if (pos < size_(n->l)) {
                n = n->l;
            } else {
                pos -= n->size;
                n = n->r;
            }
        }
        return n;
    }
    
    // Разделение по индексу (порядковой статистике).
    void split_(Link n, size_t pos, Link& l, Link& r) {
        if (!n) {
            l = r = nullptr;
        } else if (pos < n->size) {
            split_(n->l, pos, l, n->l);
            resize_(n->l);
            r = n;
        } else {
            split_(n->r, n->size - pos, n->r, r);
            resize(n->r);
            l = n;
        }
    }
    
    // Слияние поддеревьев по приоритету.
    void merge_(Link& n, Link l, Link r) {
        if (!l || !r) {
            n = l ? l : r;
        } else if (r->priority < l->priority) {
            merge_(l->r, l->r, r);
            n = l;
        } else {
            merge_(r->l, l, r->l);
            n = r;
        }
        resize_(n);
    }
    
    void destroy_(Link n) {
        if (n != nullptr) {
            destroy_(n->l);
            destroy_(n->r);
            delete n;
        }
    }
    
public:
    ImplicitTreap() {}
    
    size_t size() const { return size_(_root); }
    
    void insert(const T& item, size_t pos) {
        assert(pos <= size_(_root));
        Link l, r;
        split_(_root, pos, l, r);
        merge_(_root, l, new Node(item));
        merge_(_root, _root, r);
    }
    
    template<typename It>
    void insert(It begin, It end) {
        for (It i = begin; i != end; ++i) {
            insert(*i);
        }
    }
    
    void insertAt(std::initializer_list<T>&& il) {
        for (const T& t : il) {
            insert(t);
        }
    }
    
    void removeAt(size_t pos) {
        Link l, m, r;
        split_(_root, pos, l, r);
        split_(r, 1, m, r);
        merge_(_root, l, r);
    }
    
    T& operator[](size_t pos) { return *searchAt_(pos); }
    const T& operator[](size_t pos) const { return *searchAt_(pos); }
    
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
    ~ImplicitTreap() {
        destroy_(_root);
    }
};

void testImplicitTreap();

#endif /* ImplicitTreap_hpp */
