//
//  hashTable.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 01/04/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef hashTable_hpp
#define hashTable_hpp

#include <iostream>
#include <string>
#include <initializer_list>
#include <cassert>

// Универсальная хэш-функция для всех типов, которые тривиально копируются.
template<typename T> struct Hash {
	// Включение оператора () только для POD-типов.
	typename std::enable_if<std::is_pod<T>::value, size_t>::type
	operator() ( const T&  key, size_t capacity ) const {
		size_t size = sizeof(T); // Размер Т в char.
		const char* p = reinterpret_cast<const char*>(&key);
		size_t h = 0;
		while (size--) h = (h << 1)^*p++; // Хэш-функция Страуструпа.
		return h % capacity;
	}
};

// Специализация хэш-функции для строк.
template<> struct Hash<std::string> {
	size_t operator() ( const std::string& s, size_t capacity ) const {
		size_t size = s.size();
		size_t h = 0;
		const char* p = s.data();
		while (size--) h = (h << 1)^(*(p++)); // Хэш-функция Страуструпа. Приоритеты обозначены скобками.
		return h % capacity;
	}
};

// Хэш-таблица. Item - тип элемента. К - тип ключа.
template <typename Item, typename Key = Item, typename H = Hash<Key>> class HashTable {
	// Узел односвязного списка (цепочки) элементов с одинаковым значением хэш-функции.
	struct Node {
		Item item;
		Node* next = nullptr;
		
		Node(const Item& item, Node* next = nullptr) : item(item), next(next) {}
		~Node() { delete next; } //
	};
	typedef Node* Link;
	Link* _heads = nullptr; // массив цепочек элементов с одинаковым значением хэш-функции.
	size_t _capacity = 0; // емкость.
	size_t _count = 0; // размер таблицы.
	H _hash; // хэш-функтор.
	
	// Линейный поиск элемента в цепочке элементов с одинаковым значением хеш-функии.
	Link search_(Link l, const Key& key) const {
		if ( l == nullptr ) return nullptr;
		if ( l->item == key ) return l;
		return search_(l->next, key);
	}
	
	// Хэш-функция.
	size_t hash_( const Key& key ) const { return _hash(key, _capacity); }
	
	void deleteTable_(Link* heads, size_t capacity) {
		for ( size_t i = 0; i < capacity; i++ ) {
			delete heads[i];
		}
		delete [] heads;
	}
	
public:
	
	HashTable(size_t capacity) : _capacity(capacity) {
		_heads = new Link[_capacity] {}; // Выделяем динамический массив и заполняем массив нулями.
	}
	
	// Емкость
	size_t capacity() const { return _capacity; }
	
	// Размер
	size_t size() const { return _count; }
	
	// Поиск по ключу
	const Item* search( const Key& key ) const {
		Link l = search_( _heads[hash_(key)], key );
		return l ? &(l->item) : nullptr;
	}
		
	// Вставка
	Item& insert(Item item) {
        const Key& key = item.key();
		size_t i = hash_(key);
		Link l = search_(_heads[i], key);
		if (l == nullptr) {
			l = new Node(item, _heads[i]);
			_heads[i] = l;
			_count++;
		} else {
			l->item = item;
		}
		return l->item;
	}
		
	// Удаление.
	bool remove(const Key& key) {
		size_t i = hash_(key);
		Link p = _heads[i];
		Link n = p;
		for( ; n != nullptr && !(n->item == key);  n = n->next ) {
			if (n != p) p = p->next;
		}
		if ( n == nullptr )	return false; // не найдено.
		if ( n == p ) _heads[i] = n->next; // 1-й элемент в цепочке.
		else p->next = n->next; // не 1-й элемент в цепочке.
		
		n->next = nullptr;
		delete n;
		_count--;
		return true;
	}
    
    void remove(std::initializer_list<Key>&& keys) {
        for (auto key : keys) {
            remove(key);
        }
    }
		
	// Рехешитрование с новой емкостью
	void rehash(size_t newCapacity) {
		Link* oldTable = _heads;
		size_t oldCapacity = _capacity;
		_count = 0;
		_capacity = newCapacity;
		_heads = new Link[_capacity] {};
		for( size_t i = 0; i < oldCapacity; i++ ) {
			for( Link l = oldTable[i]; l != nullptr; l = l->next ) {
                // В этом месте используется оператор конверсии.
				insert(l->item);
			}
		}
		deleteTable_(oldTable, oldCapacity);
	}
	
	// Вывод диаграммы заполненности хэш-таблицы.
	void show() const {
		using std::cout;
		cout << "count: " << size() << std::endl;
		for( size_t i = 0; i < _capacity; i++ ) {
			cout << "bucket"<< i <<": ";
			for( Link l = _heads[i]; l != nullptr; l = l->next ) {
				cout << '*';
			}
			cout << std::endl;
		}
		cout << std::endl;
	}
	
	~HashTable() {
		deleteTable_(_heads, _capacity);
	}
	HashTable(const HashTable&) = delete;
	void operator=(const HashTable&) = delete;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HashSet.

template <typename T, typename H = Hash<T>> class HashSet {
    
    struct SetItem {
        T _value; // значение, оно же ключ.
        
        const T& key() { return _value; }
        
        operator T& () {return _value;}
        operator const T& () const {return _value;}
        
        bool operator==(const T& key) const { return _value == key; }
        
        SetItem() = default;
        SetItem(const T& value) : _value(value) {}
        SetItem(T&& value) : _value (value) {}
    };

    HashTable<SetItem, T, H> _hashTable;
    
public:
    using value_type = T;
    
    HashSet(size_t initialCapacity = 10) : _hashTable(initialCapacity) {}
    
    // Емкость
    size_t capacity() const { return _hashTable.capacity(); }
    
    // Размер
    size_t size() const { return _hashTable.size(); }
    
    // Поиск по ключу
    const T* search( const T& key ) const {
        SetItem* link = _hashTable.search(key);
        return link ? &(*link) : nullptr;
    }
    
    void insert(const T& value) {_hashTable.insert(value);}
    void insert(std::initializer_list<T>&& values) { _hashTable.insert(values); }
    bool remove(const T& key) { return _hashTable.remove(key); }
    void remove(std::initializer_list<T>&& keys) { _hashTable.remove(std::move(keys)); }
	bool has(const T& t) const { return _hashTable.search(t); }
    void show() const { _hashTable.show(); }
    void rehash(size_t newSize) { _hashTable.rehash(newSize); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HashMap.
template <typename T, typename K = T, typename H = Hash<K>> class HashMap {
    
    // Элемент таблицы.
    // first - индекс элемента в массиве. Изменять недопустимо! Хотя возможно и это недоделка.
    // second - значение элемента. Можно менять как угодно.
    struct Item : std::pair<K, T> { // Пара индекс - значение.
        using base = std::pair<K, T>;
        using base::first;
        using base::second;
        
        Item() = default;
        Item(const Item&) = default;
        Item(const K& k, const T& t) : base(k, t) {}
        Item(const base& b) : base(b) {}
        
        //bool operator<(const Item& i) const { return first < i.first; }
        bool operator==(const T& key) const { return first == key; }
        
        const T& key() { return first; }
        
        operator T& () {return second;}
        operator const T& () const {return second;}

        friend std::ostream& operator << (std::ostream& os, const Item& it) { return os << "{" << it.first << ", " << it.second << "}"; }
    };

    using HashTableType = HashTable<Item, K, H>;
    HashTableType _hashTable;
    
public:
    using value_type = std::pair<K, T>;
    
    HashMap(size_t initialCapacity = 10) : _hashTable(initialCapacity) {}
    
    // Емкость
    size_t capacity() const { return _hashTable.capacity(); }
    
    // Размер
    size_t size() const { return _hashTable.size(); }

	bool has( const K& k ) { return _hashTable.search( k ); }
	
	T& operator[](const K& k) {
		Item* res = const_cast<Item*>(_hashTable.search(k));
		return res ? *res : _hashTable.insert( {k, T()} );
	}
    
    void insert(const T& value, const K& key) {_hashTable.insert({key, value});}
    void insert(std::initializer_list<value_type>&& values) {
        for (auto&& v : values) {
            _hashTable.insert(v);
        }
    }
    bool remove(const K& key) { return _hashTable.remove(key); }
    void remove(std::initializer_list<T>&& keys) { _hashTable.remove(std::move(keys)); }
    bool has(const K& key) const { return _hashTable.search(key); }
    void show() const { _hashTable.show(); }
    void rehash(size_t newSize) { _hashTable.rehash(newSize); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test.
inline void testHashTable() {
	using namespace std;
	cout << "\nset:\n";
	
	HashSet<string> set(11);
	string strs[] {"s0", "s1", "s2","s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s12","s13", "s14", "s15", "s16", "s17", "s18", "s19" };
	for( string& s : strs ) set.insert(s);
    for( string& s : strs ) assert(set.has(s));
	assert(!set.has("s22"));
	
    set.remove( {"s17", "s15", "s13", "s11"} );
    assert(!set.has("s13"));
    set.show();

    cout << "rehash to 20\n";
    set.rehash(19);
    set.show();

    cout << "rehash to 10\n";
    set.rehash(11);
    set.show();

	cout << "\nmap:\n";
	
    HashMap<string> map(5);
//
    map["k1"] = string("s1");
    cout << "k1 : " << map["k1"] << endl;

    map["k0"] = string("s0");
    cout << "k0 : " << map["k0"] << endl;

    map.insert( {{"k2", "s2"}, {"k3", "s3"}, {"k4", "s4"}, {"k5", "s5"}} );

    cout << endl;

    map.show();
    assert(map.has("k2"));
    assert(map.remove("k2"));
    assert(!map.has("k2"));
    map.remove( {"k0", "k1", "k3", "k4", "k5"} );
    map.show();
    assert(map.size() == 0);
}

#endif /* hashTable_hpp */
