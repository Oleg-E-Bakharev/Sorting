//
//  BitStream.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 12.11.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef BitStream_hpp
#define BitStream_hpp

#include <fstream>
#include <assert.h>

// Поток для вывода битов в двоичный файл.
class BitOFStream {
    std::ofstream _os;
    uint8_t _buffer = 0; // буфер бит.
    uint8_t _counter = 0; // указатель позиции в буфере.
    
    // Проверка что при переполнении _counter у нас будет полностью заполненный _buffer.
    //static_assert( 256 % sizeof(_buffer) == 0, "bufer size is invalid");
    
public:
    BitOFStream(std::string path) : _os(path, std::ios::binary | std::ios::out | std::ios::trunc) {
        assert(_os);
        if(_os) {
            _os.seekp(sizeof(_buffer)); // Резервируем место вначале для сохранения размера в битах последнего поля.
        }
        assert(_os);
    }
    
    // Размер потока в байтах.
    size_t size() { return _os.tellp() * 1 + 1; } // Без * 1  - ошибка компилятора.
    // Размер потока в битах.
    size_t sizeBits() { return _os.tellp() * 8 + _counter % sizeof(_buffer); }
    
    // выводит бит.
    void outBit (bool bit) {
        const size_t bufferSizeInBits = sizeof(_buffer) * 8;
        // Накапливаем биты от старшего к младшему до байта и выводим байт в поток.
        if (!_os) return;
        _counter++; // Увеличиваем счетчик битов в буфере.
        _buffer <<= 1; // смещаем буфер влево, освобождая место под следующий бит. В младшем бите 0.
        if (bit) _buffer |= 1; // Если выводим 1, то младший бит устанавливаем в 1.
        if (_counter % bufferSizeInBits == 0) { // через каждые sizeof(_buffer) итераций выводим _buffer в поток.
            // Выводим буфер в поток. При этом буфер полностью обновился с момента прошлого вывода.
            _os << _buffer;
        }
    }
    
    // Выводит байт.
    void outByte(char c) {
        for (int i = 7; i >= 0; i--) {
            outBit((c & 0x80) > 0);
            c <<= 1;
        }
    }
    
    operator bool() const { return bool(_os); }
    
    ~BitOFStream() {
        if(_os) {
            // Дописываем последний буфер (если есть) и сохраняем в 0-й позиции количество информативных бит последнего буфера.
            _counter %= sizeof(_buffer) * 8; // Количество информативных бит буфера.
            if (_counter > 0) {
                _os << _buffer;
            }
            _os.seekp(0);
            _os << _counter;
        }
    }
};

// Поток для ввода битов из двоичного файла, созданного классом BitOFStream.
class BitIFStream {
    std::ifstream _is;
    uint8_t _buffer = 0; // буфер бит.
    uint8_t _next = 0; // следующий символ за буффером.
    uint8_t _counter = 0; // счетчик битов буфера.
public:
    BitIFStream(std::string path) : _is(path, std::ios::binary | std::ios::in) {
        assert(_is);
        if (_is) {
            // Поскольку eof возводится только после неудачной попытки чтения символа, следующего за последним,
            // то нам нужно читать на один символ дальше текущей позиции чтобы правильно обнаружить последний символ.
            _is.seekg(sizeof(_buffer));
            _next = _is.get();
        }
    }
    
    // Получить бит из потока.
    bool inBit() {
        const size_t bufferSizeInBits = sizeof(_buffer) * 8;
        // Читаем слово из потока и возвращаем биты от старшего к младшему.
        if (!bool(*this)) return false;
        if (_counter % bufferSizeInBits == 0) {
            _buffer = _next;
            _next = _is.get();
        }
        // Всегда извлекаем старший бит слова.
        bool result = (_buffer & (1 << (bufferSizeInBits - 1))) > 0;
        _buffer <<= 1; // смещаем 1-цу в буффере на разряд влево.
        _counter--; // Уменьшаем счетчик битов в буффере.
        
        if (_is.eof()) {
            _is.clear();
            // Читаем первое слово.
            _is.seekg(0);
            assert(_is);
            _counter = _is.get();
            _buffer <<= bufferSizeInBits - _counter - 1;
            _is.setstate(std::ios_base::failbit);
            assert(!_is);
        }
        
        return result;
    }
    
    char inByte() {
        char c = 0;
        for (int i = 0; bool(*this) && i < 8; i++) {
            c <<= 1;
            c |= inBit() ? 1 : 0;
        }
        return c;
    }
    
    operator bool() const {
        return _is.good() || _counter != 0;
    }
};

void testBitStream();

#endif /* BitWriter_hpp */
