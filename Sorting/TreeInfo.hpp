//
//  TreeInfo.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 04.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef treeTraits_hpp
#define treeTraits_hpp

#include <iostream>
#include <iomanip>

// Вспомогательный класс для получения информации о дереве.
template <typename Link> class TreeInfo {
    const Link& _root;
    
    // Количество узлов дерева.
    size_t size_(Link n) const {
        if (n == nullptr) {
            return 0;
        }
        return size_(n->r) + 1 + size_(n->l);
    }
	
	// Высота дерева.
	size_t height_(Link n) const {
		if (n == nullptr) {
			return 0;
		}
		return std::max(height_(n->l), height_(n->r)) + 1;
	}
	
	// Максимальное количество узлов на одной высоте.
	size_t width_(size_t h) const {
		size_t w = 0;
		while(h != -1) {
			w = std::max(w, levelWidth_(_root, h) );
			h--;
		}
		return w;
	}
	
	// Количество узлов на заданной высоте.
	size_t levelWidth_(Link n, size_t l) const {
		if (n == nullptr) {
			return 0;
		}
		if (l == 0) {
			return 1;
		}
		return levelWidth_(n->l, l - 1) + levelWidth_(n->r, l - 1);
	}
    
    void out_(Link n, std::ostream& os) const {
        using namespace std;
        int w = (int)os.width();
        if(n->r) {
            os << setw(w + 1);
            out_(n->r, os);
        }
        os << setw(w * 4) << "" << *n << endl;
        if(n->l) {
            os << setw(w + 1);
            out_(n->l, os);
        }
    }

public:
	TreeInfo(const Link& root):_root(root) {}
	
    size_t size() const {
        return size_(_root);
    }
    
	size_t height() const {
		return height_(_root);
	}
	
	size_t width() const {
		return width_(_height(_root));
	}
	
	void info(std::ostream& os) const {
        auto& root = *_root;
        size_t n = sizeof(root);
        size_t s = size();
		size_t h = height();
		size_t w = width_(h);
		
        os << "Sizeof Node:" << n << " Size:" << s << " Heigh:" << h << " Width:" << w << std::endl;
		os << *this;
	}
	
	friend std::ostream& operator << (std::ostream& os, const TreeInfo& t) {
		using namespace std;
        if (t._root) {
            t.out_(t._root, os);
        } else {
            os << "<null>";
        }
		return os << endl;
	}
};

template <typename Link> TreeInfo<Link> treeInfo( Link const& t) { return TreeInfo<Link>(t); }

#endif /* treeTraits_hpp */
