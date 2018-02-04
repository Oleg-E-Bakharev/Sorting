//
//  tree.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 04.08.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef tree_hpp
#define tree_hpp

#include <iostream>
#include <initializer_list>
#include "TreeInfo.hpp"

template <typename T, typename K = T> class BinaryTree {
	
	struct Node {
		T item; // Элемет дерева.
		Node* l; // левое поддерево.
		Node* r; // правое поддерево.
		
		Node(const T& item):item(item), l(nullptr), r(nullptr) {}
		
		friend std::ostream& operator << (std::ostream& os, const Node& it) {
			// Оператор вывода дерева в поток. os.width используется как величина отступа для вывода уровня узла дерева.
			os << it.item;
			return os;
		}
	};
	
	using Link = Node*;
	
	Link _root = nullptr; // корень дерева.
	
	Link search_(Link n, K key) const {
		if (n == nullptr) return nullptr; // провал.
		if (n->item == key) return n; // успех.
		if (key < n->item) {
			return search_(n->l, key);
		}
		return search_(n->r, key);
	}
	
	void insert_(Link& n, const T& t) {
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
	
	// Вставляет t в корень дерева n.
	void insertRoot_(Link& n, const T& t) {
		// Как обычная вставка но после каждого прохода в потомок поднимаем его на один уровень вверх.
		// вставка элементка на k-й уровень исходного дерева приведет к k подъемам этого узла наверх.
		// В результате вставленный узел окажется корнем.
		if (n == nullptr) {
			n = new Node(t);
			return;
		}
		if (t < n->item) {
			insertRoot_(n->l, t);
			rotR_(n);
		} else {
			insertRoot_(n->r, t);
			rotL_(n);
		}
	}
	
	// В поддереве n помещает в корень минимальный элемент (у которого l = nullptr).
	void _minToRoot(Link& n) {
		// Очень похоже на вставку в корень но без вставки. Рассуждения - те же самые.
		if (n == nullptr || n->l == nullptr) {
			return;
		}
		_minToRoot(n->l);
		rotR_(n);
	}
	
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
	
	// Делает l левым поддеревом r. l,r - потомки одного узла.
	Link joinLR_(Link l, Link r) {
		if (r == nullptr) {
			return l;
		}
		// Поднимаем минимальный узел r в его корень.
		// После этого левле поддерево r становится пустым.
		// Цепляем к нему l.
		_minToRoot(r);
		assert(r->l == nullptr);
		r->l = l;
		return r;
	}

	void destroy_(Link n) {
		if (n != nullptr) {
			destroy_(n->l);
			destroy_(n->r);
			delete n;
		}
	}
	
public:
	void insert(const T& item) {
		insert_(_root, item);
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
	
	void insertRoot(const T& item) {
		_insertRoot(_root, item);
	}
	
	bool remove(const K& key) {
		return removeKey_(_root, key);
	}
	
	T* search(const K& key) const {
		return search_(_root, key);
	}
	
	bool has(const K& key) const {
		return search(key) != nullptr;
	}
	
	TreeInfo<Link> info() const { return treeInfo(_root); }
	
	~BinaryTree() {
		destroy_(_root);
	}
};

void testTree();

#endif // tree_hpp
