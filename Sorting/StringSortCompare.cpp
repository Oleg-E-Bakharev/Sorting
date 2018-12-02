//
//  StringSortCompare.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 22.10.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "StringSortCompare.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include "common.h"
#include "LSD2.hpp"
#include "LSD.hpp"
#include "LSD3.hpp"
#include "MSD.hpp"
#include "MSD2.hpp"
//#include "MSDQSortDebug.hpp"
#include "MSDQSort.hpp"

using namespace std;

vector<string> generateRandomStrings(size_t count, size_t length) {
	vector<string> strings;
	strings.reserve(count);
	
	default_random_engine rand{random_device{}()};
	uniform_int_distribution<char> randomChar(0x61, 0x7A); // a..z
	for (size_t i = 0; i < count; ++i) {
		string s(length, ' ');
		for (size_t j = 0; j < length; ++j) {
			s[j] = randomChar(rand);
		}
		strings.push_back(s);
	}
	
	return strings;
}

class StringDigitizer {
	mutable size_t _step = 0;
	size_t _length;
public:
	using DigitType = uint8_t;
	const size_t size = 0x7B-0x61; // Размер словаря подсчёта.
    const static size_t DigitRange = 0x7B-0x61;
	inline size_t width() const { return _length; }
	inline size_t step() const { return _step; }
	inline void setStep(size_t step) const {_step = step;}

	inline size_t operator()(const string& s) const { assert(_step < s.length()); return s[_step] - 0x61;	}
	inline size_t operator()(const string& s, size_t step) const {
		if (step < s.size()) return s[step];
		return 0;
	}

    static inline char digit (const std::string& str, size_t pos) {
        return (pos < str.size()) ? str[pos] : 0;
    }
    static inline char digitIndex (const std::string& str, size_t pos) {
        return (pos < str.size()) ? str[pos] - 0x61 : 0;
    }

	StringDigitizer(size_t length) : _length(length) {}
};

void stringSortCompare()
{
	const size_t N = 4;
//	StringDigitizer digitizer(N);
	cout << "Runing stress test\n";
//	while(true) {
		auto v = generateRandomStrings(100000, N);
		auto c = v;
		auto engine = std::default_random_engine{ std::random_device{}() };
		
	cout << "std::sort performance: " << measureSort(v, engine, [&v]{ std::sort(v.begin(), v.end()); }) << endl;
	cout << "msd qsort performance: " << measureSort(v, engine, [&v]{ msdQSort(v.begin(), v.end(), StringDigitizer(N)); }) << endl;
    cout << "msd qsortBM performance: " << measureSort(v, engine, [&v]{ msdQSortBM(v.begin(), v.end(), StringDigitizer(N)); }) << endl;
//    cout << "lsd  performance: " << measureSort(v, engine, [&v]{
//        vector<string*>result = lsdSort(v.begin(), v.end(), StringDigitizer(N));
//        vector<string> temp;
//        temp.reserve(v.size());
//        for(size_t i = 0; i < v.size(); ++i) temp.push_back(*result[i]);
//        for(size_t i = 0; i < v.size(); ++i) v[i] = temp[i];
//    }) << endl;
//
//    cout << "lsd2  performance: " << measureSort(v, engine, [&v]{
//        vector<string*>result = lsdSort2(v, StringDigitizer(N));
//        vector<string> temp;
//        temp.reserve(v.size());
//        for(size_t i = 0; i < v.size(); ++i) temp.push_back(*result[i]);
//        for(size_t i = 0; i < v.size(); ++i) v[i] = temp[i];
//    }) << endl;
	
    cout << "lsd  performance: " << measureSort(v, engine, [&v]{
        lsdSort3(v.begin(), v.end(), StringDigitizer(N));
    }) << endl;

//    cout << "msd  performance: " << measureSort(v, engine, [&v]{
//        vector<string*>result = msdSort(v.begin(), v.end(), StringDigitizer(N));
//        vector<string> temp;
//        temp.reserve(v.size());
//        for(size_t i = 0; i < v.size(); ++i) temp.push_back(*result[i]);
//        for(size_t i = 0; i < v.size(); ++i) v[i] = temp[i];
//    }) << endl;

    cout << "msd  performance: " << measureSort(v, engine, [&v]{
        msdSort2(v.begin(), v.end(), StringDigitizer(N));
    }) << endl;

//		vector<string*>result = msdSort(v.begin(), v.end(), StringDigitizer(N));
//		cout << "\npointers:\n";
//		for( auto& s : result) cout << *s << "\n";
//
//		vector<string> temp;
//		temp.reserve(v.size());
//		for(size_t i = 0; i < v.size(); ++i) temp.push_back(*result[i]);
//		for(size_t i = 0; i < v.size(); ++i) v[i] = temp[i];
//
//		if (!is_sorted(v.begin(), v.end())) {
//			cout << "\ninitial:\n";
//			for( auto& s : c) cout << "\"" << s << "\",\n";
//			cout << "\nsorted:\n";
//			for( auto& s : v) cout << s << "\n";
//			break;
//		}
//	}
}
