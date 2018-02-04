//
//  for_iter.h
//  Sorting
//
//  Created by Oleg Bakharev on 08/04/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef for_iter_h
#define for_iter_h

// Универсальный итератор, который преобразовывает любой random-access-iterable класс в sequence-iterable-class
// Для использования в range-based for
// Для использования T должен поддерживать [size_t] и size()
// А также тип reference как ссылка на хранимый тип.
template <class T> class for_iter_t {
    T& t;
    size_t pos;
public:
    for_iter_t(T& t) : t(t), pos(0) {}
//    for_iter_t& begin() { pos = 0; return *this;}
//    for_iter_t& end() {return *this;}
    typename T::reference operator*() { return t[pos]; }
    bool operator != (const for_iter_t& f) const { assert(&f.t == &t); return pos != t.size(); }
    void operator++() { ++pos; }
};

// Функция для инстанциирования for_iter_t чтобы параметр шаблона вывелся компилятором.
template<class T> for_iter_t<T> for_iter(T& t) { return for_iter_t<T>(t); };

#endif /* for_iter_h */
