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
#include <queue>
#include <vector>
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
            if (it.reversed) {
                os << '*';
            }
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
    
    static void inverseReversed_(Link n) {
        if (n) n->reversed ^= true;
    }
    
    static void push_(Link n) {
        if (!n) return;
        if (!n->reversed) return;
        std::swap(n->l, n->r);
        n->reversed = false;
        inverseReversed_(n->l);
        inverseReversed_(n->r);
    }
    
    // Порядковая статистика.
    Link searchAt_(size_t pos) const {
        assert(pos < size_(_root));
        Link n = _root;
        while(n) {
            push_(n);
            if (size_(n->l) == pos) break;
            if (pos < size_(n->l)) {
                n = n->l;
            } else {
                pos -= size_(n->l) + 1;
                assert(pos < size());
                n = n->r;
            }
        }
        return n;
    }
    
    // Разделение по индексу (порядковой статистике).
    void split_(Link n, size_t pos, Link& l, Link& r) {
        push_(n);
        
        if (!n) {
            l = r = nullptr;
        } else if (pos <= size_(n->l)) {
            split_(n->l, pos, l, n->l);
            r = n;
            resize_(r);
        } else {
            split_(n->r, pos - size_(n->l) - 1, n->r, r);
            l = n;
            resize_(l);
        }
    }
    
    // Слияние поддеревьев по приоритету.
    void merge_(Link& n, Link l, Link r) {
        push_(l);
        push_(r);
        
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
    
    // Итератор для прохода по дереву в порядке возрастания индексов за O(n).
    class Iterator {
        Link _current = nullptr; // Текущий элемент.
        
        std::vector<Link> _depth; // стек обхода дерева в глубину.
        
        // Выбрать следующий индекс.
        bool selectNext_(Link n) {
            if (n == nullptr) return false;
            push_(n);
            _depth.push_back(n);
            assert(_depth.size() > 0);
            if (!selectNext_(n->l)) {
                _current = n;
            }
            return true;
        }
        
    public:
        Iterator(Link root) { selectNext_(root); }
        
        void operator++() {
            _depth.pop_back();
            if (!selectNext_(_current->r)) {
                _current = _depth.back();
            }
        }
        
        T& operator*() {
            return _current->item;
        }

        bool operator!=(const Iterator& it) { return _depth.size() != 0; }
    };
    
    friend class Iterator;

public:
    using iterator = Iterator;
    
    ImplicitTreap() {}
    ImplicitTreap(std::initializer_list<T>&& il) { insert(std::move(il)); }
    
    size_t size() const { return size_(_root); }
    
    void insertAt(const T& item, size_t pos) {
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
    
    void insert(std::initializer_list<T>&& il) {
        for (const T& t : il) {
            insertAt(t, size());
        }
    }
    
    void removeAt(size_t pos) {
        Link l, m, r;
        split_(_root, pos, l, r);
        split_(r, 1, m, r);
        assert(size_(m) == 1);
        merge_(_root, l, r);
    }
    
    void shiftLeft(size_t offset) {
        Link l, r;
        split_(_root, offset % size(), l, r);
        merge_(_root, r, l);
    }
    
    void shiftRight(size_t offset) {
        Link l, r;
        split_(_root, size() - offset % size(), l, r);
        merge_(_root, r, l);
    }

    void reverse(size_t from, size_t to) {
        assert(from < to && to <= size());
        Link l, m, r;
        split_(_root, from, l, r);
        split_(r, to - from, m, r);
        m->reversed ^= true;
        merge_(l, l, m);
        merge_(_root, l, r);
    }
    
    void reverse() {
        reverse(0, size());
    }
    
    T& operator[](size_t pos) { return searchAt_(pos)->item; }
    const T& operator[](size_t pos) const { return *searchAt_(pos)->item; }
    
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
    ~ImplicitTreap() {
        destroy_(_root);
    }
    
    // поддержка range based for
    iterator begin() const { return iterator(_root); }
    iterator end() const { return iterator(nullptr); }
    
    friend std::ostream& operator<<(std::ostream& os, const ImplicitTreap& t) {
        os << "[";
        for (const auto& i : t) {
            os << i << ", ";
        }
        return os << "]\n";
    }
};

void testImplicitTreap();

#endif /* ImplicitTreap_hpp */
