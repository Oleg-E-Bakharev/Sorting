//
//  SplayTree.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 25.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef SplayTree_hpp
#define SplayTree_hpp

#include <iostream>
#include "TreeInfo.hpp"
#include <assert.h>

template <typename T, typename K = T> class SplayTree {
    
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
	const T* _itemToInsert = nullptr; // указатель на элемент вставки.
    
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
	
    // Универсальная скошенная операция. При успехе поднимает результирующий узел в корень.
    // Параметризуется целевой функцией (search_, insert_, remove_).
    template <Link (SplayTree::*F)(Link& n)> Link splay_(Link& n, const K& key) {
        // Необходимо просматривать не  только детей, но и внуков.
        if (n == nullptr || n->item == key) return (this->*F)(n); // Нашли элемент или дошли до листа.
        Link result = nullptr;
        if (key < n->item) {
            Link& l = n->l;
            if (l == nullptr || l->item == key) result = (this->*F)(l);
            else if ( key < l->item ) {
                if ((result = splay_<F>(l->l, key))) rotR_(n); // zig-zig rotR n а не l
            } else if ((result = splay_<F>(l->r, key))) rotL_(l);
            if (result) rotR_(n);
        } else {
            Link& r = n->r;
            if (r == nullptr || r->item == key) result = (this->*F)(r);
            else if (key > r->item) {
                if ((result = splay_<F>(r->r, key))) rotL_(n); // zig-zig rotL n а не l
            } else if ((result = splay_<F>(r->l, key))) rotR_(r);
            if (result) rotL_(n);
        }
        return result;
    }
	
	// Функция-параметр splay_ для поиска.
	Link search_(Link& n) { return n == nullptr ? nullptr : n; }
	
	// Функция-параметр splay_ для вставки / замены элемента.
	Link insert_(Link& n) {
		if (n == nullptr) {
			n = new Node(*_itemToInsert);
		} else n->item = std::move(*_itemToInsert);
		return n;
	}
	
	// Исключение попытки удалить несуществующий ключ.
	class RemoveError : std::exception{};

	// Функция-параметр spaly_ для удаления ключа. В случае неудачи кидает исключение.
	Link remove_(Link& n) {
		if (n == nullptr) throw RemoveError();
		Link temp = n;
		n = joinLR_(n->l, n->r);
		delete temp;
		return n;
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
		_itemToInsert = &item;
		splay_<&SplayTree::insert_>(_root, item);
		_itemToInsert = nullptr;
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
		try {
        	return splay_<&SplayTree::remove_>(_root, key);
		} catch (RemoveError& e) {
			return false;
		}
		return true;
    }
    
    T* search(const K& key) {
        Link result = splay_<&SplayTree::search_>(_root, key);
        return result ? &result->item : nullptr;
    }
    
    bool has(const K& key) const {
        return search(key) != nullptr;
    }
		
    TreeInfo<Link> info() const { return treeInfo(_root); }
    
	~SplayTree() {
		destroy_(_root);
	}
    
    SplayTree() = default;
    SplayTree(const SplayTree&) = delete;
    SplayTree& operator=(const SplayTree&) = delete;
};

void testSplayTree();

#endif /* SplayTree_hpp */

//    Link splaySearch_(Link& n, K key) {
//        // Скошенный поиск. При успешном поиске поднимает найденый узел в корень.
//        // Необходимо просматривать не  только детей, но и внуков.
//        //
//        if (n == nullptr) return nullptr; // провал.
//        if (n->item == key) return n; // успех.
//        Link found = nullptr;
//        if (key < n->item) {
//            Link& l = n->l;
//            if (l == nullptr) return nullptr;
//            if (l->item == key) found = l;
//			else if (l->l != nullptr && key < l->item) {
//				if ((found = splaySearch_(l->l, key))) rotR_(n); // rotR n а не l
//			} else if (l->r != nullptr && (found = splaySearch_(l->r, key))) rotL_(l);
//            if (found) rotR_(n);
//        } else {
//            Link& r = n->r;
//            if (r == nullptr) return nullptr;
//            if (r->item == key) found = r;
//			else if (r->r != nullptr && key > r->item) {
//				if ((found = splaySearch_(r->l, key))) rotL_(n); // rotR n а не l
//			} else if (r->l != nullptr && (found = splaySearch_(r->l, key))) rotR_(r);
//            if (found) rotL_(n);
//        }
//        return found;
//    }
