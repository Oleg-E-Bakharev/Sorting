//
//  MSDQSort.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 13.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

//#include "MSDQSortDebug.hpp"
#include "MSDQSort.hpp"
#include <string>
#include <algorithm>

using namespace std;

struct StringDigitizer {
	char operator()(const string& str, size_t pos) const {
		if (pos < str.size()) return str[pos];
		return 0;
	}
};

void testMSDQSort()
{
	vector<string> strs {
//		"rwa",
//		"zat",
//		"mtj",
//		"mxn",
		
//		"jib",
//		"jpg",
//		"mxn",
//		"alk",
//		"scb",
//		"mtj",
//		"fus",
//		"rwa",
//		"zat",
//		"jic",
				"she",
		"sells",
		"seashells",
		"by",
		"the",
		"sea",
		"shore",
		"the",
		"shells4",
		"shells2",
		"shells1",
		"shells3",
		"she",
		"sells",
		"are",
		"surely",
		"seashells"
	};
	
	cout << "\nInitial array:\n";
	for (auto& str : strs) { cout << str << "\n"; }; cout << "\n";
	
	msdQSort(strs.begin(), strs.end(), StringDigitizer{});
	
	cout << "\nSorted array:\n";
	for (auto& str : strs) { cout << str << "\n"; }; cout << "\n";
	
	if (!is_sorted(strs.begin(), strs.end())) {
		cout << "Bad! Bad!\n";
	}
}
