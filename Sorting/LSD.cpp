//
//  LSD.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 15.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "LSD.hpp"

union IPAddress {
	uint32_t value;
	struct {
		uint8_t _4 : 8;
		uint8_t _3 : 8;
		uint8_t _2 : 8;
		uint8_t _1 : 8;
	};
	
	IPAddress(uint32_t ip = 0) : value(ip) {}
	
	friend std::ostream& operator<<(std::ostream& os, const IPAddress& ip) {
		return os << (int)ip._1 << "." << (int)ip._2 << "." << (int)ip._3 << "." << (int)ip._4 << "\n";
	}
};

// Сортировщик IP адресов.
class IPDistribution {
	mutable size_t _step = 0;
public:
    using DigitType = uint8_t;
	const size_t size = 256; // Размер словаря подсчёта.
	size_t width() const { return 4; }
    size_t step() const { return _step; }
	void setStep(size_t step) const {_step = step;}

	size_t operator()(const IPAddress& s) const {
		switch(_step) {
			case 0: return s._1;
			case 1: return s._2;
			case 2: return s._3;
		}
		return s._4;
	}
    
//    size_t operator()(const IPAddress* s) const { return (*this)(*s); }
};

void testLSD() {
	using namespace std;
	cout << "Test LSD sort\n";
//	IPAddress ip{0xaabbccdd};
//	auto _1 = ip._1;
//	auto _2 = ip._2;
//	auto _3 = ip._3;
//	auto _4 = ip._4;
	
	vector<IPAddress> ips {
		0xbbddccbb,
		0xbbddcccc,
		0xbbddccaa,
		0xbbddccdd,
		0xaabbccdd,
		0xccbbddaa,
		0xddbbddcc,
		0xddbbcccc,
		0xddbbbbcc,
		0xddbbaacc,
		0x12345678,
		0x87654321,
	};
	
	cout << "\nInitial array:\n";
	for (auto& ip : ips) { cout << ip; };
	
	cout << "\nSorted array:\n";
	for (const auto* ip : lsdSort(ips.begin(), ips.end(), IPDistribution{})) { cout << *ip; }
	
	return;
}
