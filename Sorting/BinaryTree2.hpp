//
//  CountBinaryTree2.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 03/11/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef CountBinaryTree2_hpp
#define CountBinaryTree2_hpp

#include <iostream>
#include "TreeInfo.hpp"
#include <random>

// Двоичное дерево Седжвика (Без указателя parent в узле). Со счетчиком потомков в узлах.
template <typename T, typename K = T> class BinaryTree2 {
    
    struct Node {
        T item;
        Node* l;
        Node* r;
        size_t c = 1; // Размер семьи (количество детей + 1)
        
        Node(T item):item(item), l(nullptr), r(nullptr) {}
        
        size_t lCnt() { return l != nullptr ? l->c : 0; }
        size_t rCnt() { return r != nullptr ? r->c : 0; }
        
        void updateCnt() { c = lCnt() + 1 + rCnt(); }

        friend std::ostream& operator << (std::ostream& os, const Node& it) {
            // Оператор вывода дерева в поток. os.width используется как величина отступа для вывода уровня узла дерева.
            os << it.item << ":" << it.c;
            return os;
        }
    };
    
    // Везде запись Link& означает "крючок для подвешивания дерева". Либо _root, либо Node::l, либо Node::r.
    using Link = Node*;
    
    Link _root = nullptr;
    std::default_random_engine _random;
    
    Link search_(Link n, K key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (key < n->item) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
    }
    
    void insert_(Link& h, T t) {
        if (h == nullptr) {
            h = new Node(t);
            return;
        }
        if ( _random() < _random.max() / h->c) {
            insertRoot_(h, t);
            return;
        }
        if (t < h->item) {
            insert_(h->l, t);
        } else {
            insert_(h->r, t);
        }
        h->updateCnt();
    }
    
    // Поворот влево. h->r становится на место h.
    void rotL_(Link& h) {
        Link x = h->r;
        h->r = x->l;
        x->l = h;
        h->updateCnt();
        x->updateCnt();
        h = x;
    }
    
    // Поворот вправо. h-l становится на место h.
    void rotR_(Link& h) {
        Link x = h->l;
        h->l = x->r;
        x->r = h;
        h->updateCnt();
        x->updateCnt();
        h = x;
    }
    
    // Вставляет t в корень дерева n.
    void insertRoot_(Link& h, T t) {
        if (h == nullptr) {
            h = new Node(t);
            return;
        }
        if (t < h->item) {
            insertRoot_(h->l, t);
            rotR_(h);
        } else {
            insertRoot_(h->r, t);
            rotL_(h);
        }
    }
    
    // Объединяет два дерева l и r в одно и подъвешивает его в на крючок h.
    void merge_(Link& h, Link l, Link r) {
        if (!l || !r) {
            h = l ? l : r;
        } else {
            if ( _random() / (_random.max() / (l->c + r->c) + 1) < l->c ) { // формула из Седжвика пр. 13.4
                merge_(l->r, l->r, r);
                h = l;
            } else {
                merge_(r->l, l, r->l);
                h = r;
            }
            h->updateCnt();
        }
    }
    
    bool removeKey_(Link& h, K key) {
        if (!h) {
            return false;
        }
        if (h->item == key) {
            Link temp = h;
            merge_(h, h->l, h->r);
            delete temp;
            return true;
        }
        bool result = false;
        if (key < h->item) {
            result = removeKey_(h->l, key);
        } else {
            result = removeKey_(h->r, key);
        }
        if (result) {
            h->updateCnt();
        }
        return result;
    }
    
    void destroy_(Link n) {
        if (n != nullptr) {
            destroy_(n->l);
            destroy_(n->r);
            delete n;
        }
    }
    
    // Вернет узел k-го элемента в порядке возрастания или nullptr
    Link select_(Link n, size_t k) {
        if (!n) {
            return nullptr;
        }
        size_t lCnt = n->lCnt();
        if (lCnt == k) {
            return n;
        }
        if (lCnt > k) {
            return select_(n->l, k);
        }
        return select_(n->r, k - lCnt - 1);
    }
    
    // Подвесит дерево за k-й элемент на крючок h.
    void partition_(Link& h, size_t k) {
        if (!h) {
            return;
        }
        size_t lCnt = h->lCnt();
        if (lCnt > k) {
            partition_(h->l, k);
            rotR_(h);
        } else if (lCnt < k) {
            partition_(h->r, k - lCnt - 1);
            rotL_(h);
        }
    }
    
    // Идеальная рекурсивная балансировка узла h
    void balance_(Link& h) {
        if (!h || h->c == 1) {
            return;
        }
        partition_(h, h->c / 2);
        balance_(h->l);
        balance_(h->r);
    }
    
public:
    BinaryTree2() {
        std::random_device rd;
        _random = std::default_random_engine(rd());
    }

    void insert(T item) {
        insert_(_root, item);
    }
    
    void insert(T* t, size_t n) {
        for (size_t i = 0; i < n; i++) {
            insert(t[i]);
        }
    }
    
    void insertRoot(T item) {
        _insertRoot(_root, item);
    }
    
    void balance() {
        balance_(_root);
    }
    
    bool remove(K key) {
        return removeKey_(_root, key);
    }
    
    bool search(T& t, K key) const {
        return search_(_root, key);
    }
    
    bool has(K key) const {
        T t;
        return search(t, key);
    }
    
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
    ~BinaryTree2() {
        destroy_(_root);
    }
};

void testBinaryTree2();

#endif /* CountBinaryTree2_hpp */
