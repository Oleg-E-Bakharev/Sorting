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

template<typename It, typename D>
inline It medianPos(It begin, It end, size_t pos) {
    auto mid = begin + ((end - begin) >> 1);
    if (D::digit(*end, pos) < D::digit(*begin, pos)) std::swap(begin, end);
    if (D::digit(*mid, pos) < D::digit(*begin, pos)) std::swap(begin, mid);
    if (D::digit(*end, pos) < D::digit(*mid, pos)) std::swap(mid, end);
    return mid;
}

// Dist item(at, pos).
template<typename It, typename D>
class MSDQSorter {
    
	inline std::pair<It, It> msdPartition_(It begin, It end, size_t pos) {
		using std::swap;
		swap(*begin, *medianPos<It, D>(begin, end - 1, pos)); // стаивм медиану первым элементом.
		auto pivot = D::digit(*begin, pos);
		for (It i = begin + 1; i != end;) {
			auto ch = D::digit(*i, pos);
			if (ch < pivot)
				swap(*begin++, *i++);
			else if (ch > pivot)
				swap(*i, *--end);
			else ++i;
		}
		// begin .. end диапазон ch == pivot.
		return {begin, end};
	}
	
	void msdSort_(It begin, It end, size_t pos) {
        
        switch (end - begin) {
            case 0:
            case 1:
                return;
                
            case 2:
                if (*begin > *(end - 1)) {
                    std::swap(*begin, *(end - 1));
                }
                return;
                
            default:
                break;
        }

		std::pair<It,It> mid = msdPartition_(begin, end, pos);
		
		msdSort_(begin, mid.first, pos);
		if (D::digit(*mid.first, pos) != 0) {
			msdSort_(mid.first, mid.second, pos + 1);
		}
		msdSort_(mid.second, end, pos);
	}
	
public:
	MSDQSorter(It begin, It end) {
		msdSort_(begin, end, 0);
	}
};

template<typename It, typename Digitizer>
void msdQSort(It begin, It end, const Digitizer& digit) {
	MSDQSorter<It, Digitizer> msd(begin, end);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bentley & McIlroy'93
template<typename It, typename D>
class MSDQSorterBM {
    
    inline std::pair<It,It> msdPartition_(It begin, It end, size_t pos) {
        using std::swap;
        It first = medianPos<It, D>(begin, end - 1, pos);
        swap(*begin, *first); // стаивм медиану первым элементом.
        auto pivot = D::digit(*begin, pos);
        It last = end;
        first = begin++;
        It left = begin; // левая позиция элементов равных опорному.
        It right = end; // правая позиция элементов равных опорному.
        
        while (true) {
            while (D::digit(*--end, pos) > pivot); // Слева цепочка элементов ограничена опорным.
            while (D::digit(*begin, pos) < pivot && end - begin > 0) ++begin; // Справа необходимо явное условие.
            if (end - begin < 1) {
                break;
            }
            swap(*begin, *end);
            
            // Все элементы равные опорному группируем на концах отрезка.
            if(D::digit(*begin, pos) == pivot) swap(*left++, *begin++);
            if(D::digit(*end, pos) == pivot) swap(*--right, *end);
        }
        begin = end;
        if (D::digit(*end, pos) == pivot) {
            end--;
        }
        
        while (right != last) {
            swap(*++begin, *--last);
        }
        
        if (end - left < 0) {
            // Эвристика на случай когда все пивоты вначале.
            return {first, begin + 1};
        }
        
        while (first != left) {
            swap(*first++, *end--);
        }
        
        return {end + 1, begin + 1};
    }
    
    void msdQSort_(It begin, It end, size_t pos) {
        
        switch (end - begin) {
            case 0:
            case 1:
                return;
                
            case 2:
                if (*begin > *(end - 1)) {
                    std::swap(*begin, *(end - 1));
                }
                return;
                
            default:
                break;
        }
        
        std::pair<It,It> mid = msdPartition_(begin, end, pos);
        
        msdQSort_(begin, mid.first, pos);
        if (D::digit(*mid.first, pos) != 0) {
            msdQSort_(mid.first, mid.second, pos + 1);
        }
        msdQSort_(mid.second, end, pos);
    }
    
public:
    MSDQSorterBM(It begin, It end) {
        msdQSort_(begin, end, 0);
    }
};

template<typename It, typename Digitizer>
void msdQSortBM(It begin, It end, const Digitizer& digit) {
    MSDQSorterBM<It, Digitizer> mds(begin, end);
}


void testMSDQSort();

#endif /* MSDQSort_hpp */
