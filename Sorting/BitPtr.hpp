//
//  BitPtr.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 09/11/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef BitPtr_hpp
#define BitPtr_hpp

// Указатель, который хранит 1 бит информации в младшем разряде.
// Почему это работает: https://en.wikipedia.org/wiki/Tagged_pointer
// https://habrahabr.ru/post/149012/

template <typename T>
class BitPtr {
    T* _ptr = 0;
    
    bool getBit_() const { return uintptr_t(_ptr) & 1; }
    
    T* getPtr_() const { return (T*)(uintptr_t(_ptr) & ~1); }
    
    void join_(T* ptr, bool bit) {
        if (bit) {
            _ptr = (T*)(uintptr_t(ptr) | 1);
        } else {
            _ptr = (T*)(uintptr_t(ptr) & ~1);
        }
    }
    
public:
    BitPtr(T* ptr, bool bit = false) { join_(ptr, bit); }
    BitPtr( const BitPtr& ptr) : _ptr(ptr._ptr) {}
    BitPtr( const BitPtr&& ptr) : _ptr(ptr._ptr) {}
    
    // Доступ к биту.
    bool bit() const { return getBit_(); }
    void setBit(bool bit) { join_(_ptr, bit); }
    T* ptr() const { return getPtr_(); }
    
    // Разываенование.
    T& operator*() { return *(getPtr_()); }
    const T& operator*() const { return *(getPtr_()); }
    
    // Обращение по указателю.
    T* operator->() { return getPtr_(); }
    const T* operator->() const { return getPtr_(); }
    
    // Присваивание.
    BitPtr& operator=(const BitPtr& p) { _ptr = p._ptr; return *this; }
    BitPtr& operator=(const BitPtr&& p) { _ptr = p._ptr; return *this; }
    BitPtr& operator=(T* p) { join_(p, getBit_()); return *this; }
    
    // Сравнение.
    bool operator==(const BitPtr& p) const { return _ptr == p._ptr; }
    bool operator==(T* p) const { return getPtr_() == p; }
    
    // Унарная логика.
    bool operator!() const { return getPtr_() == nullptr; }
    operator bool() const { return getPtr_() != nullptr; }
};

#endif /* BitPtr_hpp */
