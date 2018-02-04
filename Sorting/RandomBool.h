//
//  RandomBool.h
//  Sorting
//
//  Created by Oleg Bakharev on 30.10.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef randomBool_h
#define randomBool_h

#include <cstdint>
#include <random>

//#include <iostream>

// http://stackoverflow.com/questions/35358501/what-is-performance-wise-the-best-way-to-generate-random-bools
class RandomBool {
	// Details: http://xorshift.di.unimi.it/ There is a comparison table in the middle of the page,
	// showing that mt19937_64 is 2 times slower and is systematic.
	
	// The state must be seeded so that it is not everywhere zero. */
	std::random_device rd;
	
	uint64_t s[2] = { (uint64_t(rd()) << 32) ^ (rd()), (uint64_t(rd()) << 32) ^ (rd()) };
	uint64_t curRand = 0;
	uint8_t bit = 63;
	
	uint64_t xorshift128plus(void) {
		uint64_t x = s[0];
		uint64_t const y = s[1];
		s[0] = y;
		x ^= x << 23; // a
		s[1] = x ^ y ^ (x >> 17) ^ (y >> 26); // b, c
		return s[1] + y;
	}
	
public:
	operator bool() {
		if (bit >= 63) {
			curRand = xorshift128plus();
			bit = 0;
			return curRand & 1;
		}
		bit++;
		return curRand & (1<<bit);
	}
};

#endif /* randomBool_h */
