//
//  tree.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 04.08.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef tree2_hpp
#define tree2_hpp

#include "assert.h"
#include <iostream>

// Двоичное дерево Седжвика (Без указателя parent в узле).
template <typename T, typename K = T> class RBTree2 {
    struct Node {
        T item;
        Node* l;
        Node* r;
        bool red;
        
        Node(T item):item(item), l(nullptr), r(nullptr), red(true) {}
    };
    
    using Link = Node*;
    
    Link _root = nullptr;
    
    bool isRed_(Link n) const {
        if (n == nullptr) {
            return false;
        }
        return n->red;
    }
    
    void setRed_(Link n, bool set) {
        if (n == nullptr) {
            assert(set == false);
            return;
        }
        n->red = set;
    }

    Link search_(Link n, K key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (key < n->item) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
    }
    
    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool insert_(Link& n, Link& p, T t) {
        if (n == nullptr) {
            n = new Node(t);
            return !isRed_(p);
        }
        
        if (t < n->item) {
            if (!insert_(n->l, n, t)) {
                return fixInserL(n, p);
            }
            return true;
        } else {
            if (!insert_(n->r, n, t)) {
                return fixInserR(n, p);
            }
        }
        
        return true;
    }
    
    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool fixInserL(Link& n, Link&p) {
        assert(isRed_(n));
        if (!isRed_(n->l)) { // Перед нами был сл.1. Идем на уровень вверх.
            return !isRed_(p);
        }
        if (isRed_(p->r)) { // сл. 1 // n->l красный, n красный, p->красный.
            invert_(p);
            return false; // могут быть сл. 2/3. Надо прыгнуть вверх через 1.
        }
        // сл. 2/3.
        if (p->r == n) { // сл. 2
            rotL_(n);
        }
        // сл. 3
        rotR_(p);
        assert(!isRed_(p) && isRed_(p->l) && isRed_(p->r));

        return true;
    }
    
    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool fixInserR(Link& n, Link&p) {
        assert(isRed_(n));
        if (!isRed_(n->r)) { // Перед нами был сл.1. Идем на уровень вверх.
            return !isRed_(p);
        }
        if (isRed_(p->l)) { // сл. 1 // n->r красный, n красный, p->красный.
            invert_(p);
            return false; // могут быть сл. 2/3. Надо прыгнуть вверх через 1.
        }
        // сл.2/3.
        if (p->l == n) { // сл. 2
            rotR_(n);
        }
        // сл. 3
        rotL_(p);
        assert(!isRed_(p) && isRed_(p->l) && isRed_(p->r));
        return true;
    }
    
    // Делаем n красным, а его детей черными.
    void invert_(Link n) {
        setRed_(n, true);
        setRed_(n->l, false);
        setRed_(n->r, false);
    }
    
    // Поворот влево. n->r становится на место n.
    void rotL_(Link& n) {
        Link x = n->r;
        swapColors_(x, n);
        n->r = x->l;
        x->l = n;
        n = x;
    }
    
    // Поворот вправо. n-l становится на место n.
    void rotR_(Link& n) {
        Link x = n->l;
        swapColors_(x, n);
        n->l = x->r;
        x->r = n;
        n = x;
    }
    
    void swapColors_(Link x, Link y) {
        bool isRedX = isRed_(x);
        setRed_(x, isRed_(y));
        setRed_(y, isRedX);
    }
    
    // В поддереве n помещает в корень минимальный элемент (у которого l = nullptr).
    void _minToRoot(Link& n) {
        if (n == nullptr || n->l == nullptr) {
            return;
        }
        _minToRoot(n->l);
        rotR_(n);
    }
    
    // Делает l левым поддеревом r.
    Link joinLR_(Link l, Link r) {
        if (r == nullptr) {
            return l;
        }
        _minToRoot(r);
        assert(r->l == nullptr);
        r->l = l;
        return r;
    }
    
    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool removeKey_(Link& n, K key) {
        if (n == nullptr) {
            return false;
        }
        if (n->item == key) {
            Link temp = n;
            n = joinLR_(n->l, n->r);
            delete temp;
            return true;
        }
        if (key < n->item) {
            return removeKey_(n->l, key);
        }
        return removeKey_(n->r, key);
    }
    
    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool fixRemoveL_(Link& n, Link& p) {
        return false;
    }

    // Возвращает true если нарушений к-ч св-в в дереве нет.
    bool fixRemoveR_(Link& n, Link& p) {
        return false;
    }

    void show_(Link n, Link p, std::ostream& os, size_t h) const {
        using namespace std;
        if (n == nullptr) {
            return;
        }
        
        show_(n->r, n, os, h + 1);
        for (int i = 0; i < h; i++) {
            cout << "  ";
        }
        cout << n->item << (isRed_(n) ? 'r' : 'b');
        cout << endl;
        show_(n->l, n, os, h + 1);
        
        assert(isRed_(n) ? !isRed_(p) && !isRed_(n->l) && !(isRed_(n->r)) : true);
    }
    
    // Высота дерева.
    size_t height_(Link n) const {
        if (n == nullptr) {
            return 0;
        }
        return std::max(height_(n->l), height_(n->r)) + 1;
    }
    
    // Максимальное количество узлов на одной высоте.
    size_t width_(size_t h) const {
        size_t w = 0;
        while(h != 0) {
            w = std::max(w, levelWidth_(_root, h) );
            h--;
        }
        return w;
    }
    
    // Количество узлов на заданной высоте.
    size_t levelWidth_(Link n, size_t l) const {
        if (n == nullptr) {
            return 0;
        }
        if (l == 0) {
            return 1;
        }
        return levelWidth_(n->l, l - 1) + levelWidth_(n->r, l - 1);
    }
    
    void destroy_(Link n) {
        if (n != nullptr) {
            destroy_(n->l);
            destroy_(n->r);
            delete n;
        }
    }
    
public:
    void insert(T item) {
        insert_(_root, _root, item);
        setRed_(_root, false);
    }
    
    void insert(T* t, size_t n) {
        for (size_t i = 0; i < n; i++) {
            insert(t[i]);
        }
    }
    
    void remove(K key) {
        removeKey_(_root, key);
    }
    
    bool search(T& t, K key) const {
        return search_(_root, key);
    }
    
    bool has(K key) const {
        T t;
        return search(t, key);
    }
    
    size_t height() const {
        return height_(_root);
    }
    
    size_t width() const {
        return width_(_height(_root));
    }
    
    void info(std::ostream& os) const {
        size_t h = height();
        size_t w = width_(h);
        
        os << "H:" << h << " W:" << w << std::endl;
        os << *this;
    }
    
    // Выводит повернутое на 90 градусов дерево.
    friend std::ostream& operator << (std::ostream& os, const RBTree2& t) {
        using namespace std;
        os << endl;
        t.show_(t._root, t._root, os, 0);
        os << endl;
        return os;
    }
    
    ~RBTree2() {
        destroy_(_root);
    }
};

void testRBTree2();

#endif // tree_hpp

#pragma mark -