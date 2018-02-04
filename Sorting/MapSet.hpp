//
//  MapSet.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 20.11.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef MapSet_hpp
#define MapSet_hpp

#include <iostream>
#include "AWLTree.hpp"
#include <initializer_list>

template<typename T> class Set {
	AWLTree<T> _tree;
public:
	Set() = default;
	template<typename It> Set(const It& begin, const It& end) { insert(begin, end); }
	Set( std::initializer_list<T>&& it) : Set(it.begin(), it.end()) {}
	
	size_t size() const { return _tree.size(); }
	
	void insert(const T& t) { _tree.insert(t); }
	
	template<typename It> void insert(const It& begin, const It& end) {
		for (It i = begin; i != end; ++i) insert(*i);
	}

	bool has(const T& t) { return _tree.has(t); }
	
	void remove(const T& t) { _tree.remove(t); }
	
	class Iterator {
		const T* _ptr;
		size_t _pos;
		AWLTree<T>& _tree;
		Iterator(AWLTree<T>& tree, size_t pos) : _tree(tree), _pos(pos), _ptr(tree.select(pos)) {}
		friend class Set;
	public:
		void operator++() {
			_ptr = _tree.select(_pos + 1);
			_pos = _ptr ? _pos + 1 : _pos;
		}
		const T& operator*() { return *_ptr; }
		const T* operator->() { return _ptr; }
		
		bool operator== (const Iterator& it) { return  _ptr == it._ptr;}
		bool operator!= (const Iterator& it) { return  _ptr != it._ptr;}
	};
	
	Iterator begin() { return Iterator(_tree, 0); }
	Iterator end() { return Iterator(_tree, size()); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename K, typename T> class Map {
	struct Item : std::pair<K, T> {
        using base = std::pair<K, T>;
		using base::first;
		
        bool operator==(const Item& i) const {return first == i.first;}
		bool operator==(const K& k) const {return first == k;}
		bool operator<(const K& k) const {return first < k;}
		bool operator>(const K& k) const {return first > k;}
        
        Item(const K& k, const T& t) : base(k, t) {}
        Item(const base& b) : base(b) {}
	};
	AWLTree<Item, K> _tree;

    class Iterator {
        using Tree = AWLTree<Item, K>;
        using value_type = std::pair<K, T>;
        Tree& _tree;
        size_t _pos;
        const value_type* _ptr;
        Iterator(Tree& tree, size_t pos) : _tree(tree), _pos(pos), _ptr(tree.select(pos)) {}
        friend class Map;
    public:
        operator bool() const { return _ptr != nullptr; }
        
        const value_type& operator*() { return *_ptr; }
        const value_type* operator->() { return _ptr; }
        
        bool operator== (const Iterator& it) { return  _ptr == it._ptr;}
        bool operator!= (const Iterator& it) { return  _ptr != it._ptr;}

        void operator++() {
            _ptr = _tree.select(_pos + 1);
            _pos = _ptr ? _pos + 1 : _pos;
        }
        
        void operator+=(size_t offset) {
            _ptr = _tree.select(_pos + offset);
            _pos = _ptr ? _pos + offset : _pos;
        }
        
        Iterator operator+(size_t offset) {
            return  Iterator(_tree, _pos + offset);
        }
        
        const value_type& operator[] (size_t offset) const { return *(Iterator(_tree, _pos + offset));}
    };
    
    class Reference {
        using Tree = AWLTree<Item, K>;
        Tree& _tree;
        const K& _key;
        friend class Map;
        
        Reference (Tree& tree, const K& key)  : _tree(tree), _key(key) {}
        
    public:
		operator T&() const {
			Item* ptr = const_cast<Item*>(_tree.search(_key));
			if(!ptr) throw std::runtime_error("dereference to non exising value requested");
			return ptr->second;
		}

        void operator=(const T& value) {
            _tree.insert({_key, value});
        }
    };
public:
	using value_type = std::pair<K, T>;
    using iterator = Iterator;
    using reference = Reference;
	
	Map() = default;
	template<typename It> Map(const It& begin, const It& end) { insert(begin, end); }
	Map( std::initializer_list<std::pair<K, T>>&& it) : Map(it.begin(), it.end()) {}
	
	size_t size() const { return _tree.size(); }
	
	void insert(const K& key, const T& value) { _tree.insert(Item{key, value}); }
	void insert(const std::pair<K, T>& item) {
		_tree.insert(Item{item});
	}
	
	template<typename It> void insert(const It& begin, const It& end) {
		for (It i = begin; i != end; ++i) insert(*i);
	}
	
	bool has(const K& k) { return _tree.has(k); }
	
	void remove(const K& k) { _tree.remove(k); }
	
	iterator begin() { return Iterator(_tree, 0); }
	iterator end() { return Iterator(_tree, size()); }
    
    reference operator[](const K& k) {
        return Reference(_tree, k);
    }
};

void testMapSet();

#endif /* MapSet_hpp */
