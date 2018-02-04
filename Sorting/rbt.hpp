//
//  rbt.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 24/03/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#ifndef rbt_hpp
#define rbt_hpp

#include <iostream>
#include <string.h>
#include <assert.h>

// Красно-черное дерево. Удовлетворяет следующим условиям:
// 1. Все узлы либо красные, либо черные.
// 2. Корень дерева - черный.
// 3. Листья дерева - черные.
// 4. Если узел красный, то его родитель и оба ребенка - черные.
// 5. Количество черных узлов на любом кратчайшем пути из узла к его листу-потомку одинаково.
template <class Item>
class RBTree {
    
    struct Node {
        Item* i; // Ключ
        Node* p; // Родитель
        Node* l; // Левый потомок
        Node* r; // Правый потомок
        bool c; // Цвет: 0 - черный, 1 - красный.
        
        Node( Node* null = nullptr, Item* i = nullptr ) : i(i), p(null), l(null), r(null), c(0) {}
//        int key() { return int(*i); }
        
        bool isLeft() { return p->l == this; }
        bool isRight() { return  p->r == this; }
        Node* setLeft( Node* l ) {
			Node* t = this->l;
            this->l = l;
            l->p = this;
			return t;
        }
        Node* setRight( Node* r ) {
			Node* t = this->r;
            this->r = r;
            r->p = this;
			return t;
        }
		
        Node* cousine() {
            if ( isLeft() ) return p->r;
            return p->l;
        }
        
        Node* uncle() { return p->cousine(); }
        bool isBlack() { return c == 0; }
        bool isRed() { return c == 1; }
        void setBlack() { c = 0; }
        void setRed() { c = 1; }
		
		// Св-во 4.
		void test_rb() { if ( isRed() ) assert( p->isBlack() && l->isBlack() && r->isBlack() ); }
    };
	
	// Фиктивный узел являющийся одновременно и родителем root и всеми листьями.
    Node null{};
    
    typedef Node* Link;
    Link root = &null;
    
    void setRoot( Link x ) {
        root = x;
		x->p = &null;
    }
    
    void destroy_( Link x ) {
        if ( x == root ) root = &null;
		if ( x != &null ) delete x;
    }
    
    int count_( Link x ) {
        if ( x == &null ) return 0;
        return count_(x->l) + count_( x->r ) + 1;
    }
    
    int height_( Link x ) {
        if ( x == &null ) return -1;
        int hl = height_(x->l), hr = height_( x->r );
        return hl > hr ? hl + 1 : hr + 1;
    }
	
    Link search_( Link x, Item& key ) {
        while (x != &null && *x->i != key)
            if (key < *x->i) x = x->l;
            else x = x->r;
        return x;
    }
    
    Link min_( Link x ) {
        while (x->l != &null) x = x->l;
        return x;
    }
    
    Link insertionPoint( Item& key ) {
        Link x = root;
        Link y = &null;
        while ( x != &null ) {
            y = x;
            if ( key < *x->i ) x = x->l;
            else x = x->r;
        }
        return y;
    }
    
    // Заменить x на y. X выбрасываем.
    Link transpant( Link x, Link y ) {
        if ( x->p == &null ) setRoot( y );
        else if ( x->isLeft() ) x->p->setLeft( y );
        else x->p->setRight(y);
		return y;
    }
    
    // Обход в ширину (поперечный) с указанием высоты.
    void traverse_( Link x, int h, void(*traverseProc)(Item* x, int h, bool isRed) ) {
		if ( x == &null )  {
//			traverseProc( nullptr, h, x->isRed() );
			return;
		}
        traverse_( x->r, h + 1, traverseProc );
        traverseProc( x->i, h, x->isRed() );
        traverse_( x->l, h + 1, traverseProc );
    }
	
	// Методы красно черных деревьев.
	void rotate_left( Link x ) {
		Link y = x->r;
		Link p = x->p;
		if ( x->isLeft() ) p->setLeft( y );
		else p->setRight( y );
		Link z = y->setLeft( x );
		x->r = z;
		if( z != &null ) z->p = x; // У нулевого узла родителя не меняем! Иначе в сл3. удаления получим сбой.
		if ( root == x ) { root = y; }
	}

	void rotate_right( Link x ) {
		Link y = x->l;
		Link p = x->p;
		if ( x->isLeft() ) p->setLeft( y );
		else p->setRight( y );
		Link z = y->setRight( x );
		x->l = z;
		if ( z != &null ) z->p = x; // У нулевого узла родтеля не меняем!
		if ( root == x ) { root = y; }
	}

	typedef void ( RBTree::*rotate_f )( Link x );
	// Случаи - из Кормена.
	// возврат: true - остановиться, false - продолжить.
	bool insert_fix_(Link x, rotate_f rotate_l, rotate_f rotate_r, bool moveX) {
		Link y = x->uncle();
		if ( y->isRed() ) { // дядя красный. Сл. 1.
			y->setBlack();
			x->p->setBlack();
			y->p->setRed();
			return false;
		}
		if ( moveX ) { // Мы должны продвинуть x. Сл. 2.
			x = x->p;
			(this->*rotate_l)( x );
		}
		x->p->setBlack(); // Сл. 3.
		x->p->p->setRed();
		(this->*rotate_r)( x->p->p );
		return true;
	}
	
	void insert_fix( Link x ) {
		while ( x->p->isRed() ) {
			if ( x->p->isLeft() ) {
				if ( insert_fix_( x, &RBTree::rotate_left, &RBTree::rotate_right, x->isRight() ) ) break;
			}
			else if ( insert_fix_( x, &RBTree::rotate_right, &RBTree::rotate_left, x->isLeft() ) ) break;
			x = x->p->p;
		}
		root->setBlack();
	}
	
	typedef Link Node::*dir;
	
	// Случаи из Кормена.
	Link delete_fix_( Link x, rotate_f rotate_l, rotate_f rotate_r, dir left, dir right ) {
		Link y = x->p->*right;
		if ( y->isRed() ) { // брат красный. Сл.1.
			y->setBlack();
			x->p->setRed();
			(this->*rotate_l)(x->p);
			y = x->p->*right; // новый брат.
		}
		if ( y->l->isBlack() && y->r->isBlack() ) { // У нового брата оба ребенка черные. Сл.2
			y->setRed();
			return x->p; // продвигаемся на уровень вверх.
		}
		if ( (y->*right)->isBlack() ) { // у нового брата левый - красный, правый черный. Сл.3
			(y->*left)->setBlack();
			y->setRed();
			(this->*rotate_r)(y);
			y = x->p->*right;
		}
		y->c = x->p->c; // у нового брата правый красный. Сл.4.
		x->p->setBlack();
		(y->*right)->setBlack();
		(this->*rotate_l)(x->p);
		return root; // Выходим из цикла.
	}
	
	void delete_fix( Link x ) {
		while ( x != root && x->isBlack() ) {
			if ( x->isLeft() ) x = delete_fix_( x, &RBTree::rotate_left, &RBTree::rotate_right, &Node::l, &Node::r);
			else x = delete_fix_( x, &RBTree::rotate_right, &RBTree::rotate_left, &Node::r, &Node::l);
		}
		x->setBlack();
	}
	
	// Тест св-в 4, 5. Возвращает черную высоту дерева.
	int test_( Link x ) {
		int h = height_(x);
		if ( x == &null ) return 0;
		x->test_rb(); // 4.
		int bhl = test_(x->l), bhr = test_( x->r );
		assert(bhl == bhr); // 5.
		if ( x->isBlack() ) bhl++;
		assert(h <= 2 * bhl); // Основное св-во КЧ-деревьев.
		return bhl;
	}
	
public:
    RBTree() {}
    ~RBTree() { destroy_(root); }
    
    int count() { return count_(root); }
    
    int height() { return height_(root); }
    
    Item* search( Item& key ) {
        Link x = search_( root, int(key) );
        if ( x != nullptr ) return &x->i;
        return nullptr;
    }
	
    void insert( Item& i ) {
		int c = count();
        Link p = insertionPoint( i );
        Link x = new Node( &null, &i );
		if ( p == &null ) setRoot( x );
        else {
            x->p = p;
            if ( int(i) < *p->i ) p->l = x;
            else p->r = x;
        }
		x->setRed();
		insert_fix( x );
		assert( count() == c + 1 );
		test();
    }
    
    void insert( Item* Items, int n ) { for( int i = 0; i < n; i++ ) insert( Items[i] ); }
    
    bool remove(Item& i) {
		int c = count();
        Link x = search_( root, i );
		Link z = nullptr;
		if ( x == &null ) return false;
		// Только в одном узле может произойти нарушение св-ва 5.
		bool isRemovedBlack = x->isBlack();
        if ( x->l == &null ) z = transpant( x, x->r );
        else if ( x->r == &null ) z = transpant( x, x->l );
        else {
            Link y = min_( x->r );
			z = y->r;
			isRemovedBlack = y->isBlack();
			
            if ( y->p == x ) {
				z->p = y;
			} else {
                transpant( y, y->r );
                y->setRight( x->r );
            }
            transpant( x, y );
            y->setLeft( x->l) ;
			y->c = x->c;
        }
		if ( isRemovedBlack ) {
			delete_fix( z );
		}
		assert( count() == c - 1 );
		test();
        delete x;
        return true;
    }
    
    void removeAll() { destroy_(root); }
    
    // Обход в ширину (Поперечный обход).
    typedef void(*TraverseProc)(Item* x, int h, bool isRed);
    void traverse(TraverseProc proc) {
        traverse_(root, 0, proc);
    }
	
	// Вывод дерева, повернутого на 90 градусов.
    void show() {
        using namespace std;
        cout << endl;
        traverse( []( Item* x, int h, bool isRed ) {
            for (int i = 0; i < h; i++) cout << "  ";
			cout << (isRed ? 'r' : 'b');
            if (x != nullptr) cout << *x;
            else cout << "*";
            cout << endl;
        } );
        cout << endl;
    }
	
	// Тест.
	void test() {
		assert( root->isBlack() );
		test_(root);
	}
};

namespace RBT {

void testRBT();
	
}

#endif /* rbt_hpp */