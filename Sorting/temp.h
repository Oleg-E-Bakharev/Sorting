#include "assert.h"
#include <iostream>

// Двоичное дерево Седжвика (Без указателя parent в узле).
template <typename T, typename K = T> class Tree {
    struct Node {
        T item;
        Node* l;
        Node* r;
        
        Node(T item):item(item), l(nullptr), r(nullptr) {}
    };
    
    using Link = Node*;
    
    Link _head = nullptr;
    
    Link search_(Link n, K key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (key < n->item) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
    }
    
    void insert_(Link& n, T t) {
        if (n == nullptr) {
            n = new Node(t);
            return;
        }
        if (t < n->item) {
            insert_(n->l, t);
        } else {
            insert_(n->r, t);
        }
    }
    
    // Поворот влево. n->r становится на место n.
    void rotL_(Link& n) {
        Link x = n->r;
        n->r = x->l;
        x->l = n;
        n = x;
    }
    
    // Поворот вправо. n-l становится на место n.
    void rotR_(Link& n) {
        Link x = n->l;
        n->l = x->r;
        x->r = n;
        n = x;
    }
    
    // В поддереве n помещает в корень минимальный элемент (у которого l = nullptr).
    void minToHead_(Link& n) {
        if (n == nullptr) {
            return;
        }
        minToHead_(n->l);
        rotR_(n);
    }
    
    // Делает l левым поддеревом r.
    Link joinLR_(Link l, Link r) {
        if (r == nullptr) {
            return l;
        }
        minToHead_(r);
        assert(r->l == nullptr);
        r->l = l;
        return r;
    }
    
    bool removeKey_(Link& n, K key) {
        if (n == nullptr) {
            return false;
        }
        if (n->item == key) {
            Link temp = n;
            n = joinLR_(n->l, n->r);
            if (temp == _head) {
                _head = n;
            }
            delete temp;
            return true;
        }
        if (key < n->item) {
            return removeKey_(n->l, key);
        }
        return removeKey_(n->r, key);
    }
    
    void show_(Link n, std::ostream& os, int h) const {
        using namespace std;
        if (n == 0) {
            return;
        }
        
        show_(n->l, os, h + 1);
        for (int i = 0; i < h; i++) {
            cout << "  ";
        }
        cout << n->item;
        cout << endl;
        show_(n->r, os, h + 1);
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
        insert_(_head, item);
    }
    
    void insert(T* t, size_t n) {
        for (size_t i = 0; i < n; i++) {
            insert(t[i]);
        }
    }
    
    bool remove(K key) {
        return removeKey_(_head, key);
    }
    
    bool search(T& t, K key) {
        return search_(_head, key);
    }
    
    bool has(K key) {
        T t;
        return search(t, key);
    }
    
    friend std::ostream& operator << (std::ostream& os, const Tree& t) {
        using namespace std;
        os << endl;
        t.show_(t._head, os, 0);
        os << endl;
        return os;
    }
    
    ~Tree() {
        destroy_(_head);
    }
};
