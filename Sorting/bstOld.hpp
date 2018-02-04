#ifndef bst_hpp
#define bst_hpp

#include <iostream>
#include <assert.h>

template <class Item>
class BSTree {
    
    struct Node {
        Item& i;
        Node* p;
        Node* l;
        Node* r;
        Node(Item& i) : i(i), p(nullptr), l(nullptr), r(nullptr) {}
        int key() { return i.key(); }
        
        bool isLeft() { return p->l == this; }
        bool isRight() { return  p->r == this; }
        void setLeft(Node* l) {
            assert(l != this);
            this->l = l;
            if (l != nullptr ) l->p = this;
        }
        void setRight(Node* r) {
            assert(r != this);
            this->r = r;
            if( r != nullptr) r->p = this;
        }
    };
    
    typedef Node* Link;
    Link root;
    
    void setRoot(Link x) {
        root = x;
        if(x != nullptr) x->p = nullptr;
    }
    
    void destroy_(Link x) {
        if (x != nullptr) {
            destroy_(x->l);
            destroy_(x->r);
            if (x == root) root = nullptr;
            delete x;
        }
    }
    
    int count_(Link x) {
        if (x == nullptr) return 0;
        return count_(x->l) + count_(x->r) + 1;
    }
    
    int height_(Link x) {
        if (x == nullptr) return -1;
        int hl = height_(x->l), hr = height_(x->r);
        return hl > hr ? hl + 1 : hr + 1;
    }
    
    Link search_(Link x, int key) {
        while (x != nullptr && x->key() != key)
            if (key < x->key()) x = x->l;
            else x = x->r;
        return x;
    }
    
    Link min_(Link x) {
        while (x->l != nullptr) x = x->l;
        return x;
    }
    
    Link max_(Link x) {
        while (x->r != nullptr) x = x->r;
        return x;
    }
    
    Link insertionPoint(int key) {
        Link x = root;
        Link y = nullptr;
        while (x != nullptr) {
            y = x;
            if (key < x->key()) x = x->l;
            else x = x->r;
        }
        return y;
    }
    
    void transpant(Link x, Link y) {
        if (x->p == nullptr) setRoot(y);
        else if (x->isLeft()) x->p->setLeft(y);
        else x->p->setRight(y);
    }
    
    // Обход в ширину (поперечный) с указанием высоты.
    void traverse_(Link x, int h, void(*traverseProc)(Item* x, int h)) {
        if (x == nullptr) {
            traverseProc(nullptr, h);
            return;
        }
        traverse_(x->r, h + 1, traverseProc);
        traverseProc(&x->i, h);
        traverse_(x->l, h + 1, traverseProc);
    }
    
public:
    BSTree() : root(nullptr) {}
    ~BSTree() { destroy_(root); }
    
    int count() { return count_(root); }
    
    int height() { return height_(root); }
    
    Item* search(int key) {
        Link x = search_(root, key);
        if (x != nullptr) return &x->i;
        return nullptr;
    }
    
    void insert(Item& i) {
        Link p = insertionPoint(i.key());
        Link x = new Node(i);
        if (p == nullptr) setRoot(x);
        else {
            x->p = p;
            if (i.key() < p->key()) p->l = x;
            else p->r = x;
        }
    }
    
    void insert(Item* Items, int n) { for(int i = 0; i < n; i++) insert(Items[i]); }
    
    bool remove(int key) {
        Link x = search_(root, key);
        if (x == nullptr) return false;
        if (x->l == nullptr) transpant(x, x->r);
        else if (x->r == nullptr) transpant(x, x->l);
        else {
            Link y = min_(x->r);
            if (y->p != x) {
                transpant(y, y->r);
                y->setRight(x->r);
            }
            transpant(x, y);
            y->setLeft(x->l);
        }
        delete x;
        return true;
    }
    
    void removeAll() { destroy_(root); }
    
    // Обход в ширину (Поперечный обход).
    typedef void(*TraverseProc)(Item* x, int h);
    void traverse(TraverseProc proc) {
        traverse_(root, 0, proc);
    }

    void show() {
        using namespace std;
        cout << endl;
        traverse([](Item* x, int h) {
            for (int i = 0; i < h; i++) cout << "  ";
            if (x != nullptr) cout << *x;
            else cout << "*";
            cout << endl;
        });
        cout << endl;
    }
};

namespace BST {

void testBST();

}
#endif /* bst_hpp */