//
//  BitMemoryStream.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 18/11/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "BitMemoryStream.hpp"
#include <iostream>

void testBitMemoryStream() {
    using namespace std;
    
    char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz \n0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz \n0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz \n0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    C[0] = '\0'; // проверка на допустимость символа '\0'.
    const int N = sizeof(C)/sizeof(*C);

    cout << "write:\n";
    for( char c : C ) {
        if (c == 0) {
            cout << '*';
        }
        cout << c;
    }
    cout << endl;

    
    string buf;
    
    {
        BitOSStream bs;
        for( char c : C) {
            bs.outByte(c);
        }
        // 0x5F
        bs.outBit(true);
        bs.outBit(false);
        bs.outBit(true);
        bs.outBit(true);
        bs.outBit(true);
        bs.outBit(true);
        bs.outBit(true);
        
        cout << "totally out " << bs.sizeBits() << " bits\n";
        buf = bs.close();
    }
    
    // Буффер выводится без нулевых символов!
    cout << "buffer:\n" << buf << endl;
    
    cout << "read:\n";
    
    char R[N+1] = {0};
    
    {
        BitISStream bs(buf);
        size_t pos = 0;
        while(bs) {
            R[pos++] = bs.inByte();
        }
        char c = R[pos-1];
        assert(c == 0x5F);
        assert(pos == N + 1);
    }
    
    for( char r : R ) {
        if (r == 0) {
            cout << '*';
        }
        cout << r;
    }
    cout << endl;
    
    for( int i = 0; i < N; i++ ) {
        assert(C[i] == R[i]);
    }
}
