//
//  RandomBST.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 24/08/2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef randomBST_hpp
#define randomBST_hpp

//clang 3.7.0

#include <iostream>
#include <iomanip>
#include <assert.h>
#include <cstdint>
#include <random>
#include "TreeInfo.hpp"

////////////////////////////////////////////////////////////////////////////////////////////////

// Рандомизированное двоичное дерево на основе дерамиды (Декартово дерево).
// http://e-maxx.ru/algo/treap
// https://habrahabr.ru/post/101818/
// При удалении элемента может случиться увеличение высоты дерева. Пример:
//          2
//      0        6
//       1     5   8
//           3
// Если в этом дереве удалить 2 то высота при любом раскладе увеличится на 1.
// Теоретически высота дерева при удалении элемента может увеличиваться почти в 2 раза.
// Что создает препятствие для использования удаления в этом дереве поскольку
// при длительной последовательности вставок/удалений дерево неизбежно будет разбалансироваться.

template <typename T, typename K = T> class RandomBinaryTree {
    
    struct Node {
        T item;
        Node* l = nullptr;
        Node* r = nullptr;
        
        Node(const T& item):item(item) {}
        
        friend std::ostream& operator << (std::ostream& os, const Node& it) {
            os << it.item;
            return os;
        }
    };
    
    typedef Node* Link;
    
	std::default_random_engine _random;
    size_t _size = 0;
    
    Link _root = nullptr;
    
    Link search_(Link n, const K& key) const {
        if (n == nullptr) return nullptr;
        if (n->item == key) return n;
        if (key < n->item) {
            return search_(n->l, key);
        }
        return search_(n->r, key);
    }
    
    void split_(Link n, K key, Link& l, Link& r) {
        if (!n) {
            l = r = nullptr;
        } else if (key < n->item) {
            split_(n->l, key, l, n->l);
            r = n;
        } else {
            split_(n->r, key, n->r, r);
            l = n;
        }
    }
    
    void insert_(Link& n, Link it, size_t size) {
        // Идея состоит в том, что вероятность вставки нового узла в корень поддерева из N узлов
        // должна быть 1/(N + 1). Но поскольку у нас нет данных о точном количестве потомков,
        // делаем предположение об идеальной сбалансированности дерева.
        if (!n) {
            n = it;
        }
        else if (size == 0 || _random() < _random.max() / size) {
            split_(n, it->item, it->l, it->r);
            n = it;
        } else {
            insert_(it->item < n->item ? n->l : n->r, it, size >> 1);
        }
    }
    
    void merge_(Link& n, Link l, Link r, size_t cntL, size_t cntR) {
        if (!l || !r) {
            n = l ? l : r;
        } else if (cntR < cntL) {
            merge_(l->r, l->r, r, cntL >> 1, cntR);
            n = l;
        } else {
            merge_(r->l, l, r->l, cntL, cntR >> 1);
            n = r;
        }
    }
    
    bool removeKey_(Link& n, const K& key) {
        if (!n) {
            return false;
        }
        if (n->item == key) {
            merge_(n, n->l, n->r, _size >> 1, _size >> 1);
            return true;
        }
        return removeKey_(key < n->item ? n->l : n->r, key);
    }
    
    void destroy_(Link n) {
        if (n != nullptr) {
            destroy_(n->l);
            destroy_(n->r);
            delete n;
        }
    }
    
public:
    RandomBinaryTree() : _random(std::random_device{}()) {}
	
    void insert(const T& item) {
        ++_size;
        insert_(_root, new Node(item), _size);
    }
    
	template<typename It>
	void insert(It begin, It end) {
		for (It i = begin; i != end; ++i) {
			insert(*i);
		}
	}
	
	void insert(std::initializer_list<T>&& il) {
		for (const T& t : il) {
			insert(t);
		}
	}

	bool remove(const K& key) {
		if (removeKey_(_root, key)) {
			--_size;
			assert(_size >= 0);
			return true;
		}
		return false;
	}

    bool search(const K& key) const {
        return search_(_root, key);
    }
    
    bool has(const K& key) const {
        return search(key);
    }
    
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
    ~RandomBinaryTree() {
        destroy_(_root);
    }
};

void testRandomBST();

#endif /* randomBST_hpp */
