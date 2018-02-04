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
// Ключ должен быть доступен как Item::key().
// Тип значения элемента должен быть доступен как typename Item::ItemType
// Тип ключа должен быть доступен как typename Item::KeyType
// Значение элемента должно быть доступно как Item::ValueType
template <typename Item, typename K = typename Item::KeyType, typename H = Hash<K>> class HashTable {
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
	Link search_(Link l, const K& k) const {
		if ( l == nullptr ) return nullptr;
		if ( l->item.key() == k ) return l;
		return search_(l->next, k);
	}
	
	// Хэш-функция.
	size_t hash_( K k ) const { return _hash(k, _capacity); }
	
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
	size_t capacity() { return _capacity; }
	
	// Размер
	size_t size() { return _count; }
	
	// Поиск по ключу
	const typename Item::ValueType* search( const K& k ) const {
		Link l = search_( _heads[hash_(k)], k );
		return l ? &(l->item.value()) : nullptr;
	}
	
	// Поиск по ключу
	typename Item::ValueType* search( const K& k ) {
		Link l = search_( _heads[hash_(k)], k );
		return l ? &(l->item.value()) : nullptr;
	}
	
	// Вставка
	typename Item::ValueType& insert( const Item& t ) {
		K k = t.key();
		size_t i = hash_(k);
		Link l = search_(_heads[i], k);
		if (l == nullptr) {
			l = new Node(t, _heads[i]);
			_heads[i] = l;
			_count++;
		} else {
			l->item = t;
		}
		return l->item.value();
	}
	
	// Вставка списка
	void insert(std::initializer_list<Item> Ts) {
		for( const Item& t : Ts ) {
			insert(t);
		}
	}
	
	// Удаление.
	bool remove(const K& k) {
		size_t i = hash_(k);
		Link p = _heads[i];
		Link n = p;
		for( ; n != nullptr && n->item.key() != k;  n = n->next ) {
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
	
	// Удаление списка.
	void remove(std::initializer_list<K> Ks) {
		for( const K& k : Ks ) {
			remove(k);
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
				insert(l->item);
			}
		}
		deleteTable_(oldTable, oldCapacity);
	}
	
	// Вывод диаграммы заполненности хэш-таблицы.
	void show() {
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
template <typename T> struct SetItem {
    T _value; // значение, оно же ключ.
    
public:
    using KeyType = T;
	using ValueType = T;
	
	SetItem(const T& value) : _value(value) {}
	
	ValueType& value() {return _value;}
	const ValueType& value() const {return _value;}
    const KeyType& key() const {return _value;}
};

template <typename T, typename H = Hash<T>> class HashSet : public HashTable<SetItem<T>, T, H> {
	using base = HashTable<SetItem<T>, T, H>;
public:
	using base::base; // Использование конструктора базового класса.
	bool has(const T& t) const { return this->search( t ); }
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HashMap.
template <typename T, typename K = T> class MapItem {
    K _key; // ключ.
    T _value; // значение.
    
public:
	using KeyType = T;
    using ValueType = K;
    
    MapItem(K key, T value) : _key(key), _value(value) {}
    
    ValueType& value() {return _value;}
    const ValueType& value() const {return _value;}
    const KeyType& key() const {return _key;}
};

template <typename T, typename K = T, typename H = Hash<K>> class HashMap : public HashTable<MapItem<T,K>, K, H> {
	using base = HashTable<MapItem<T,K>, K, H>;
public:
	using base::base; // Использование конструктора базового класса.
	
	bool has( const K& k ) { return this->search( k ); }
	
	T& operator[](const K& k) {
		auto res = this->search(k);
		return res ? *res : this->insert( {k, T()} );
	}
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test.
inline void testHashTable() {
	using namespace std;
	cout << "\nset:\n";
	
	HashSet<string> set(10);
	string strs[] {"s0", "s1", "s2","s3", "s4", "s5", "s6", "s7", "s8", "s9", "s10", "s11", "s12","s13", "s14", "s15", "s16", "s17", "s18", "s19" };
	for( string& s : strs ) set.insert(s);
	for( string& s : strs ) assert(set.has(s));
	assert(!set.has("s22"));
	
	set.remove( {"s17", "s15", "s13", "s11"} );
	assert(!set.has("s13"));
	set.show();
	
	cout << "rehash to 20\n";
	set.rehash(20);
	set.show();

	cout << "rehash to 10\n";
	set.rehash(10);
	set.show();

	cout << "\nmap:\n";
	
	HashMap<string> map(5);
	
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
