//
//  hashAttay.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 26.11.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef HashArray_hpp
#define HashArray_hpp

#include <iostream>
#include <vector>
#include <initializer_list>
#include <assert.h>

// Разреженный массив на основе хэш таблицы с открытой адресацией.
// Можно использовать как будто массив изначально заполнен значениями по умолчанию для хранимого типа,
// например нулями для числовых типов.
// Доступ к элементам за O(1).
// Итерация по элементам в порядке возрастания ключей за O(1).
// В процессе итерации можно как угодно изменять значения элементов - но не их индексы!
// Между операцией вызывающей вставку элемента и итерированием автоматически происходит сортировка ссылок O(Klog(K)) -
// K - количество задействованых элементов массива.
// Идеальное решение для стортировок подсчётом и построениея гистограмм.
// Если использовать массив массивов то можно получить разреженную матрицу (возможно потиребуется доработка).
// Кстати, итератор - с произвольным доступом.
template <typename T> class HashArray {
	
	// Элемент таблицы.
	// first - индекс элемента в массиве. Изменять недопустимо! Хотя возможно и это недоделка.
	// second - значение элемента. Можно менять как угодно.
	struct Item : std::pair<size_t, T> { // Пара индекс - значение.
		using base = std::pair<size_t, T>;
		using base::first;
		
		Item() = default;
		Item(const Item&) = default;
		Item(const size_t& k, const T& t) : base(k, t) {}
		Item(const base& b) : base(b) {}
		
		bool operator<(const Item& i) const { return first < i.first; }
		
		friend std::ostream& operator << (std::ostream& os, const Item& it) { return os << "{" << it.first << ", " << it.second << "}"; }
	};
	
	bool _fixed = false; // Флаг соотвтствия _data и _refs.
	std::vector<Item> _data; // Массив данных.
	std::vector<bool> _status; // Флаги занятости ячеек _data.
	std::vector<std::reference_wrapper<Item>> _refs; // Cсылки на элементы _data в отсортированном порядке.

	// Очень желательно, чтобы размер таблицы был простым числом, тогда остатки от деления хэшкодов на размер таблицы будут
	// максимально распределены на размере таблицы. Но мы хотя бы сделаем размер таблицы гарантировано нечётным.
	inline size_t tableSize(size_t itemsCount) const { return std::max(100ul, itemsCount << 1) + 1; }
	
	inline size_t hash_(size_t pos) const { return pos % _data.size(); }
	
	void put_(size_t pos, const T& t) {
		size_t i = hash_(pos);
		for ( ; _status[i]; i = (i + 1) % _data.size()) {
			if (_data[i].first == pos) {
				_data[i].second = t;
				return;
			}
		}
		assert(_refs.size() < _data.size() >> 1);
		_fixed = false;
		_data[i] = {pos, t};
		_status[i] = true;
		_refs.push_back(_data[i]);
	}
	
	Item& get_(size_t pos) {
		for (size_t i = hash_(pos); _status[i]; i = (i + 1) % _data.size()) {
			if (_data[i].first == pos) return _data[i];
		}
		// Создаём пустой элемент и возвращаем его.
		put_(pos, T());
		return _refs.back();
	}

	void fix_() {
		if (!_fixed) {
			std::sort(_refs.begin(), _refs.end(), std::less<Item>());
			_fixed = true;
		}
	}
	
	// Ссылка.
	class Reference {
		HashArray& _parent;
		size_t _pos;
		friend class HashArray;
		
		Reference (HashArray& parent, size_t pos)  : _parent(parent), _pos(pos) {}
		
	public:
		operator T&() {
			Item& ptr = _parent.get_(_pos);
			return ptr.second;
		}
		
		void operator=(const T& value) {
			_parent.put_(_pos, value);
		}
	};

	// Итератор с произвольным доступом.
	class Iterator {
		using Pos = typename std::vector<std::reference_wrapper<Item>>::iterator;
		Pos _pos;
		Iterator(const Pos& pos) : _pos(pos) {}
		friend class HashArray;
	public:
		Item& operator*() const {return *_pos;}
		Item* operator->() const {return _pos;}
		void operator++() { ++_pos; }
		bool operator!=(const Iterator& it) const { return _pos != it._pos; }
		Iterator operator+(size_t offset) const { return Iterator(_pos + offset); }
	};
	
public:
	
	using iterator = Iterator;
	using reference = Reference;

	HashArray(size_t size) : _data(tableSize(size)), _status(_data.size()) {}
	
	// Количество используемых элементов.
	size_t size() const { return _refs.size(); }
	
	// Доступ к элементам по ссылке.
	reference operator[](size_t pos) { return Reference(*this, pos); }
	
	// Начальный итератор.
	iterator begin() {
		fix_();
		return _refs.begin();
	}
	
	// Конечный итератор.
	iterator end() {
		fix_();
		return _refs.end();
	}
};

void testHashArray();
#endif /*HashArray_hpp */
