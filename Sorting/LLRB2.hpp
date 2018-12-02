//
//  LLRB2.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 09/11/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef LLRB2_hpp
#define LLRB2_hpp

#include <iostream>
#include "TreeInfo.hpp"
#include "BitPtr.hpp"
#include <assert.h>

// Left Leaning Red Black Tree https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
// http://www.cs.princeton.edu/~rs/talks/LLRB/LLRB.pdf
// Дерево размером с двусвязный список.
template <typename T, typename K = T> class LLRB2 {
    
    struct Node {
        T item;
        BitPtr<Node> l = nullptr;
        BitPtr<Node> r = nullptr;
        
        Node(T item): item(item) {
            assert(!l && !r);
        }
        
        friend std::ostream& operator << (std::ostream& os, const Node& it) {
            // Оператор вывода дерева в поток. os.width используется как величина отступа для вывода уровня узла дерева.
            os << it.item;
            return os;
        }
    };
        
    using Link = BitPtr<Node>;
    
    Link _root = nullptr;
    
    inline bool isRed_(const Link& n) const {
        return n.bit();
    }
    
    inline void setRed_(Link& n, bool set) {
        if (!n) {
            assert(set == false);
            return;
        }
        n.setBit(set);
    }
    
    Link search_(const Link& n, K key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (key < n->item) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
    }
    
    // Меняет цвета h и его детей.
    inline void colorFlip_(Link& h) {
        setRed_(h, !isRed_(h));
        setRed_(h->l, !isRed_(h->l));
        setRed_(h->r, !isRed_(h->r));
    }
    
    // Поворот влево. h->r становится на место h. Поворачиваем только красные узлы.
    inline void rotL_(Link& h) {
        Link x = h->r;
        setRed_(x, isRed_(h));
        setRed_(h, true);
        h->r = x->l;
        x->l = h;
        h = x;
    }
    
    // Поворот вправо. h-l становится на место h. Поворачиваем только красные узлы.
    inline void rotR_(Link& h) {
        Link x = h->l;
        setRed_(x, isRed_(h));
        setRed_(h, true);
        h->l = x->r;
        x->r = h;
        h = x;
    }
    
    void insert_(Link& h, T t) {
        if (h == nullptr) {
            Node* ptr = new Node(t);
            assert(ptr != nullptr);
            h = Link(ptr, true);
            return;
        }
        
        if (t < h->item) {
            insert_(h->l, t);
        } else {
            insert_(h->r, t);
        }

        fix_(h);
    }
    
    // балансировка после вставки / удаления узла.
    inline void fix_(Link& h) {
        if (isRed_(h->r) && !isRed_(h->l)) {
            rotL_(h);
        }
        if (isRed_(h->l) && isRed_(h->l->l)) {
            rotR_(h);
        }
        if (isRed_(h->l) && isRed_(h->r)) {
            colorFlip_(h);
        }
    }
    
    inline void moveRedLeft_(Link& h) {
        colorFlip_(h);
        if (isRed_(h->r->l)) {
            rotR_(h->r);
            rotL_(h);
            colorFlip_(h);
        }
    }
    
    inline void moveRedRight_(Link& h) {
        colorFlip_(h);
        if (isRed_(h->l->l)) {
            rotR_(h);
            colorFlip_(h);
        }
    }
    
    void delMin_(Link& h) {
        // Вытягиваем дерево влево и удаляем самый левый узел.
        if (h->l == nullptr) { // Удаляем узел на нижнем уровне.
            delete h.ptr();
            h.setBit(false);
            h = nullptr;
            return;
        }
        
        if (!isRed_(h->l) && !isRed_(h->l->l)) {
            moveRedLeft_(h); // Продвигаем красноту вниз.
        }
        
        delMin_(h->l);
        fix_(h);
    }
    
    void delMax_(Link& h) {
        // Вытягиваем дерево вправо и удаляем самый правый узел.
        if (isRed_(h->l)) {
            rotR_(h);
        }
        
        if (!h->r) {
            delete h.ptr();
            h.setBit(false);
            h = nullptr;
            return;
        }
        
        if (!isRed_(h->r) && !isRed_(h->r->l)) {
            moveRedRight(h);
        }
        
        delMax_(h->l);
        fix_(h);
    }
    
    T& min_(Link& n) {
        return n->l ? min_(n->l) : n->item;
    }
    
    T& max_(Link& n) {
        return n->r ? max_(n->r) : n->item;
    }
    
    bool del_(Link& h, K key) {
        // Обходим склеивая 2-узлы.
        bool result = false;
        
        if (h == nullptr){
            return false;
        }
        
        if (key < h->item) {
            // Проход влево.
            if (!isRed_(h->l) && !isRed_(h->l->l)) {
                moveRedLeft_(h);
            }
            result = del_(h->l, key); // Идем влево.
        } else {
            if (isRed_(h->l)){
                rotR_(h);
            }
            
            if (key == h->item && !h->r) { // Если равно и внизу просто удаляем.
                delete h.ptr();
                h.setBit(false);
                h = nullptr;
                return true;
            }
            
            if (!isRed_(h->r) && !isRed_(h->r->l)) {
                moveRedRight_(h); // Продвигаем красноту вниз вправо.
            }
            
            if (key == h->item) { // Равно и не внизу меняем значение на минимальное и удаляем минимальный узел.
                h->item = std::move(min_(h->r));
                delMin_(h->r);
                result = true;
            } else {
                result = del_(h->r, key); // Идем вправо.
            }
        }
        if (result) {
            fix_(h);
        }
        return result;
    }
    
    void destroy_(Link& n) {
        if (n) {
            destroy_(n->l);
            destroy_(n->r);
            delete n.ptr();
        }
    }
    
public:
    void insert(T item) {
        insert_(_root, item);
        setRed_(_root, false);
        assert(!isRed_(_root));
    }
    
    void insert(T* t, size_t n) {
        for (size_t i = 0; i < n; i++) {
            insert(t[i]);
        }
    }
    
    const T& max() {
        assert(_root);
        return max_(_root);
    }
    
    const T& min() {
        assert(_root);
        return min_(_root);
    }
    
    bool removeMin() {
        bool result = _root ? (delMin_(_root), true) : false;
        setRed_(_root, false);
        return result;
    }
    
    bool removeMax() {
        bool result = _root ? (delMin_(_root), true) : false;
        setRed_(_root, false);
        return result;
    }
    
    bool remove(K key) {
        bool result = _root ? del_(_root, key) : false;
        setRed_(_root, false);
        return result;
    }
    
    bool search(T& t, K key) const {
        return search_(_root, key);
    }
    
    bool has(K key) const {
        T t;
        return search(t, key);
    }
    
    TreeInfo<BitPtr<Node>> info() const { return treeInfo(_root); }
    
    ~LLRB2() {
        destroy_(_root);
    }
};

void testLLRB2();

#endif /* LLRB2_hpp */
