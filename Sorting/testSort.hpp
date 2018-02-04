//
//  testSort.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 26/10/15.
//  Copyright © 2015 Oleg Bakharev. All rights reserved.
//

#ifndef testSort_hpp
#define testSort_hpp

#include <stdio.h>
#include <assert.h>

template <typename T>
void testSort(T* array, size_t size)
{
	if (size < 2) {
		return;
	}
	
	size_t length;
	
	size_t maxGrowsLength = 0;
	size_t maxGrowsBegin = 0;
	
	size_t maxDecreaseLength = 0;
	size_t maxDecreaseBegin = 0;
	
	size_t currentBegin = 0;
	size_t equalityLength = 0;
	T prevValue = array[0];
	
	bool isGrowing = true;
	
	for (int i = 1; i < size; i++) {
		T currentValue = array[i];
		if (currentValue == prevValue) {
			
			equalityLength++;
			
		} else if (isGrowing ) {
			if (currentValue < prevValue) {
				// Закончилось возрастание. Началось убывание.
				isGrowing = false;
				
				size_t currentGrowsLength = i - currentBegin;
				if (currentGrowsLength > maxGrowsLength) {
					maxGrowsLength = currentGrowsLength;
					maxGrowsBegin = currentBegin;
				}
				
				currentBegin = i - 1 - equalityLength;
			}
			equalityLength = 0;
		} else {
			if (currentValue > prevValue) {
				// Закончилось убывание. Началось возрастание.
				isGrowing = true;
				
				size_t currentDecreaseLength = i - currentBegin;
				if (currentDecreaseLength > maxDecreaseLength) {
					maxDecreaseLength = currentDecreaseLength;
					maxDecreaseBegin = currentBegin;
				}
				
				currentBegin = i - 1 - equalityLength;
			}
			equalityLength = 0;
		}
		prevValue = currentValue;
	}
	
	if (isGrowing) {
		size_t currentGrowsLength = size - currentBegin;
		if (currentGrowsLength > maxGrowsLength) {
			maxGrowsLength = currentGrowsLength;
			maxGrowsBegin = currentBegin;
		}
	} else {
		size_t currentDecreaseLength = size - currentBegin;
		if (currentDecreaseLength > maxDecreaseLength) {
			maxDecreaseLength = currentDecreaseLength;
			maxDecreaseBegin = currentBegin;
		}
	}
	
	size_t begin = 0;
	size_t end = 0;
	if (maxGrowsLength >= maxDecreaseLength) {
		begin = maxGrowsBegin;
		length = maxGrowsLength;
	} else {
		begin = maxDecreaseBegin;
		length = maxDecreaseLength;
	}
	
	end = begin + length - 1;
	assert(length == size);
}

#endif /* testSort_hpp */
