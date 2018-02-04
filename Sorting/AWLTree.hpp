//
//  AWLTree.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/11/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef AWLTree_hpp
#define AWLTree_hpp

#include <iostream>
#include "TreeInfo.hpp"
#include <algorithm>

// Двоичное дерево Седжвика (Без указателя parent в узле). Со счетчиком потомков в узлах.
template <typename T, typename K = T> class AWLTree {
    
    struct Node {
        using Link = Node*;
        T item;
        Link l;
        Link r;
        size_t c = 1; // Размер семьи (количество детей + 1)
        
        Node(T item):item(item), l(nullptr), r(nullptr) {}
        
        size_t lCnt() const { return l != nullptr ? l->c : 0; }
        size_t rCnt() const { return r != nullptr ? r->c : 0; }
        
        void updateCnt() { c = lCnt() + 1 + rCnt(); }
        
        // Выдаёт баланс а-ля AWL.
        long balance() const {
            long lSize = lCnt();
            long rSize = rCnt();
            if (lSize < rSize) {
                if (lSize == 0) return -rSize;
                return -rSize / lSize;
            }
            if (rSize == 0) return lSize;
            return lSize / rSize;
        }
        
        friend std::ostream& operator << (std::ostream& os, const Node& it) {
            // Оператор вывода дерева в поток. os.width используется как величина отступа для вывода уровня узла дерева.
            os << it.item << ":" << it.c;
            return os;
        }
    };
    
    // Везде запись Link& означает "крючок для подвешивания дерева". Либо _root, либо Node::l, либо Node::r.
    using Link = Node*;
    
    Link _root = nullptr;
    
    Link search_(Link n, const K& key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (n->item > key) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
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
    
    void fixBalance_(Link& h) {
        h->updateCnt();
        long balance = h->balance();
        if (balance < -1) {
            if (h->r->balance() > 0) rotR_(h->r);
            rotL_(h);
        } else if (balance > 1) {
            if (h->l->balance() < 0) rotL_(h->l);
            rotR_(h);
        }
    }
    
    void insert_(Link& h, const T& t) {
        if (h == nullptr) {
            h = new Node(t);
            return;
        }
		if (h->item == t) {
			h->item = std::move(t);
		} else if (t < h->item) {
            insert_(h->l, t);
        } else {
            insert_(h->r, t);
        }
        fixBalance_(h);
    }

    // Объединяет два дерева l и r в одно и подвешивает его в на крючок h.
    void merge_(Link& h, Link l, Link r) {
        if (!l || !r) {
            h = l ? l : r;
        } else {
            if ( l->c < r->c ) {
                merge_(r->l, l, r->l);
                h = r;
            } else {
                merge_(l->r, l->r, r);
                h = l;
            }
            fixBalance_(h);
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
        if (h->item > key) {
            result = removeKey_(h->l, key);
        } else {
            result = removeKey_(h->r, key);
        }
        if (result) {
            fixBalance_(h);
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
    Link select_(Link n, size_t k) const {
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
    
public:
    AWLTree() = default;
    
    void insert(const T& item) {
        insert_(_root, item);
    }
	
	size_t size() const { return _root ? _root->c : 0; }

    template<typename It>
    void insert(It begin, It end) {
        for (It i = begin; i != end; ++i) {
            insert(*i);
        }
    }
    
    bool remove(K key) {
        return removeKey_(_root, key);
    }
    
    const T* search(const K& key) const {
        Link n = search_(_root, key);
        return n ? &n->item : nullptr;
    }
    
    bool has(const K& key) const {
        return search(key) != nullptr;
    }
	
	const T* select(size_t k) const {
		Link n = select_(_root, k);
		return n ? &n->item : nullptr;
	}
    
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
    ~AWLTree() {
        destroy_(_root);
    }
    
    AWLTree(const AWLTree&) = delete;
    AWLTree& operator=(const AWLTree&) = delete;
};

void testAWLTree();

#endif /* AWLTree_hpp */
