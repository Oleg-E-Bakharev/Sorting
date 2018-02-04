//
//  MSDQSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 13.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef MSDQSort_hpp
#define MSDQSort_hpp

#include <iostream>
#include <utility>
#include <vector>
#include <assert.h>

// Dist item(at, pos).
template<typename It, typename Digitizer>
class MSDQSorter {
	const Digitizer& _digit;
	inline std::pair<It,It> msdPartition_(It begin, It end, size_t pos) {
		using std::swap;
		//		swap(*begin, *medianPos(begin, end - 1)); // стаивм медиану первым элементом.
		auto pivot = _digit(*begin, pos);
		for (It i = begin + 1; i != end;) {
			auto ch = _digit(*i, pos);
			if (ch < pivot)
				swap(*begin++, *i++);
			else if (ch > pivot)
				swap(*i, *--end);
			else ++i;
		}
		// begin .. end диавазон ch == pivot.
		return {begin, end};
	}
	
	void msdSort_(It begin, It end, size_t pos) {
		if (end - begin <= 1) return;
		
		std::pair<It,It> mid = msdPartition_(begin, end, pos);
		
		msdSort_(begin, mid.first, pos);
		if (_digit(*mid.first, pos) != 0) {
			msdSort_(mid.first, mid.second, pos + 1);
		}
		msdSort_(mid.second, end, pos);
	}
	
public:
	MSDQSorter(It begin, It end, const Digitizer& digit) : _digit(digit) {
		msdSort_(begin, end, 0);
	}
};

template<typename It, typename Digitizer>
void msdQSort(It begin, It end, const Digitizer& digit) {
	MSDQSorter<It, Digitizer> msd(begin, end, digit);
}

void testMSDQSort();

#endif /* MSDQSort_hpp */
