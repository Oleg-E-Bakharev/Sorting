//
//  hashArray.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 26.11.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "hashArray.hpp"
#include <string>

using namespace std;

void testHashArray()
{
	wstring str = L"йцукенгшщзхъёфывапролджэячсмитьбюйцукенгшщзхъёфывйцукенгшщзхъё";
	HashArray<wchar_t> ha(str.size());
	for (auto wc : str) {
		ha[wc]++;
	}
	
	cout << "Output: {unicode:count}\n";
	for (auto it : ha) {
		cout << it << ", ";
	}

	cout << "\nMediane: " << *(ha.begin() + ha.size() / 2) << "\n";
}
