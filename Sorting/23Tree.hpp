//
//  23Tree.hpp
//  Sorting
//
//  Created by Oleg Bakharev on 31.10.17.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#ifndef _3Tree_hpp
#define _3Tree_hpp

#include <iostream>
#include <assert.h>

// 2-3 дерево без указателя на родитель.
// https://habrahabr.ru/post/303374/
// https://www.cs.princeton.edu/~rs/talks/LLRB/RedBlack.pdf
template <typename T, typename K = T> class Tree23 {
	class Node {
		using Link = Node*;
		T first, second; // значения
		Link left = nullptr, middle= nullptr, right = 1.; // потомки. Если right == 1 значит second отсутствует и у нас 2-узел.
        
        // Из 3-узла делает 2-узел.
        void make2Node() {
            assert(is3Node());
            first = std::move(second);
            left = middle;
            middle = right;
            right = 1.; // Инвариант 2-узла.
            assert(is2Node());
        }
        
        friend std::ostream& operator<<(std::ostream& os, const Node& node) {
            
        }
		
		//////////////////////// Вставка ///////////////////////////
		
		// Разбивает листовой 3-узел по значению t на три - родительский (this) и два новых ребенка.
		void splitLeaf(const T& t) {
			assert(isLeaf());
			assert(is3Node());
			const T& min = std::min(first, t);
			const T& max = std::max(second, t);
			const T& mid = t < min ? min : t > max ? max : t;
			
			Link newLeft = new Node(min);
			left = newLeft;
			
			Link newMiddle = new Node(max);
			middle = newMiddle;
			
			first = mid;
			right = 1.; // инвариант 2 узла.
		}
		
		// Сливает с собой переданный 2-узел. Если узел влился в this то возвращается nullptr.
		// Иначе возвращается родитель разбиения.
		// если передать nullptr то ничего не происходит.
		Link merge(Link node) {
			if (node == nullptr) return;
			assert(node->is2Node());
			if (is2Node()) {
				if (first < node->first) {
					// делаем node вторым значением, свой middle теряется (предполагается что сливаемый узел из middle ветки.
					second = std::move(node->first);
					middle = node->left;
					right = node->middle;
					delete node;
					return nullptr;
				}
				// делаем ноде первым значением. Свой left теряется (предполагается что ливаемый узел и left ветки.
				second = std::move(first);
				first = std::move(node->first);
				right = middle;
				left = node->left;
				middle = node->middle;
				delete node;
				return nullptr;
			}
			
			// Разбиваем свой узел по узлу node и возвращаем новый родительский узел.
			return splitNonLeaf(node);
		}
		
		// Разбивает нелистовой 3-узел по 2-узлу на три - новый родительский и два ребенка.
		Link splitNonLeaf(Link node) {
			assert(!isLeaf());
			assert(is3Node());
			assert(node->is2Node());
			if (node->first < first) { // Узел пришел из левой ветки.
				Link newNode = new Node(first); // новый родитель.
				newNode->left = node;   //    this> D   E   ->           D <newNode
				newNode->middle = this; // node> B    F   G -> node> B       E <this
				make2Node();            //     A   C        ->     A   C   F   G
				return newNode;
			}
			if (node->first > second) { // Узел пришел из правой ветки.
				Link newNode = new Node(second); // новый родитель.
				newNode->left = this;     // this> B   D           ->           D <newNode
				newNode->middle = node;   //    A    C    F <node  -> this> B       F <node
				right = 1.; // инв. 2-узла.             E   G      ->     A   C   E   G
				return newNode;
			}
			assert(node->first > first && node->first < second);
			// Узел пришел из средней ветки.
			Link newRight = new Node(second); // новый правый узел.
			middle = node->left;           //
			newRight->left = node->middle; // this> B     F    ->          D <node
			newRight->middle = right;      //    A     D     G -> this> B       F <newNode
			node->left = this;             //  node> C   E     ->     A   C   E   G
			node->middle = newRight;
			right = 1.;
			
			return node;
		}
		
		Link insertLeaf(const T& t) {
			assert(isLeaf());
			if (first == t) {
				first = t; // Логика ассоциативности.
				return nullptr;
			}
			if (is2Node()) {
				second = t;
				if (second < first) std::swap(first, second);
				return nullptr;
			}
			if (second == t) {
				second = t; // Логика ассоциативности.
				return nullptr;
			}
			return splitLeaf(t);
		}
		
		Link inserNonLeaf(const T& t) {
			if (t == first) {
				first = t; // Логика ассоциативности.
				return nullptr;
			}
			if (t < first) return left->insert(t);
			if (is2Node()) return middle->insert(t);
			if (t == second) {
				second = t; // Логика ассоциативности.
				return nullptr;
			}
			if (t < second) {
				return middle->insert(t);
			}
			return right->insert(t);
		}
		
		//////////////////////// Удаление ///////////////////////////
		
        //     A    ->    -
        //  -     B -> AB   0
        // Применимо только к 2-узлам. middle удаляется.
        // После поворота баланс остаётся нарушен.
        void rotateLeftUnbalanced() {
            assert(is2Node());
            assert(middle->is2Node());
            left->first = std::move(first);
            left->second = std::move(middle->first);
            left->middle = middle->left;
            left->right = middle->middle;
            delete middle;
            middle = nullptr;
        }
        
        //     A     C   ->    C
        //  -     B    D -> AB    D
        // Применимо только к 3-узлам. middle и right 2-узлы.  После поворота баланс восстановлен.
        void singleRotateleft() {
            assert(is3Node());
            assert(middle->is2Node());
            assert(right->is2Node());
            left->first = std::move(first);
            left->second = std::move(middle->first);
            left->middle = middle->left;
            left->right = middle->middle;
            make2Node();
        }
        
        //     A   [D]    ->    B    [D]
        //  -    BC  [EF] -> A     C    [EF]
        // Применимо и к 3-узлам. После поворота баланс восстановлен.
        void middleRotateleft() {
            left->first = std::move(first);
            first = std::move(middle->first);
            left->middle = middle->left;
            if (middle->is3Node()) {
                middle->make2Node();
            }
        }
        
        //     A    C    ->    B     D
        //  -    B    DE -> A     C     E
        // Применимо только к 3-узлам. После поворота баланс восстановлен.
        void rightRotateleft() {
            assert(is3Node());
            assert(middle->is2Node());
            assert(right->is3Node());
            left->first = std::move(first);
            first = std::move(middle->first);
            left->middle = middle->left;
            
            middle->first = std::move(second);
            middle->left = right->left;
            middle->middle = right->middle;
            
            second = std::move(right->first);
            right->make2Node();
        }
        
        // Корректировка нарушения баланса левого поддерева вида:
        //    A      [D]
        // -    B[C]     [EF]
        // Возврат: true баланс восстановлен. false баланс нарушен, требуется коррекция баланса на уровне выше.
        // Если возврат false, то узел будет вида:
        //    -
        // AB   0
        bool fixLeftBalance() {
            // Необходимо скомпенсировать баланс в левом поддереве.
            assert(left != nullptr); // В балансировке нуждается только существующий левый потомок.
            assert(left->is2Node()); // Который может быть только 2-узлом.
            assert(left->middle == nullptr); // И у которого middle отсутствует.
            assert(middle != nullptr); // Если left != nullptr то по условию баланса middle != nullptr
            
            if (middle->is3Node()) { // имеем middle 3-узел - разбиваем его и выходим.
                middleRotateleft();
                return true;
            }
            if (is3Node()) {
                assert(right != nullptr); // Если left != nullptr и this 3-узел, то по условию баланса right != nullptr.
                
                if (right->is3Node()) { // имеем right 3-узел - разбиваем его и выходим.
                    rightRotateleft();
                    return true;
                }
                
                // имеем this->3 узел в котором middle и right - 2-узлы.
                singleRotateleft();
                return true;
            }
            
            // балансировка не удалась. группируем потомки в левой ветке и передаём несбалансированность выше.
            rotateLeftUnbalanced();
            return false;
        }
        
        //    C    [D]     ->    B     [D]
        // AB    -    [EF] -> A     C      [EF]
        void leftRotatemiddle() {
            middle->fisrt() = std::move(first); //    C   ->    B
            first = std::move(left->second);    // AB   - -> A     C
            middle->middle = middle->left;
            middle->left = left->right;
            left->right = 1.; // делаем left 2-узлом.
        }
        
        // Корректировка нарушения баланса в среднем поддереве:
        //      C    [D]
        // A[B]    -     [EF]
        // Возврат: true баланс восстановлен. false баланс нарушен, требуется коррекция баланса на уровне выше.
        // Если возврат false, то узел будет вида:
        //    -
        // AB   0
        bool fixMiddleBalance() {
            assert(middle != nullptr); // В балансировке нуждается только существующий средний потомок.
            assert(middle->is2Node()); // Который является 2-узлом.
            assert(middle->middle == nullptr); // И у которого middle отсутствует.
            if (is2Node()) {
                if (left->is2Node()) {               //    B   ->    -
                    left->second = std::move(first); // A    - -> AB    0
                    left->right = middle->left;
                    delete middle;
                    middle = nullptr;
                    return false;
                }
                leftRotatemiddle();
                return true;
            }
            if (left->is2Node()) {               //    B     C      ->    C
                left->second = std::move(first); // A     -    D[E] -> AB    D[E]
                left->right = middle->left;
                delete middle;
                first = std::move(second);
                middle = right;
                return true;
            }
            leftRotatemiddle();
            return true;
        }
        
        // Корректировка нарушения баланса в правом поддереве вида:
        //      C        F
        // A[B]    D[E]     -
        // Баланс всегда можно скорректировать.
        void fixRightBalance() {
            assert(is3Node()); // Применимо только к 3-узлам.
            assert(right != nullptr); // В балансировке нуждается только существующий правый потомок.
            assert(right->is2Node()); // Который является 2-узлом.
            assert(right->middle == nullptr); // И у которого middle отсутствует.
            if (middle->is2Node()) {
                middle->second = std::move(second); //      C     E    ->      C
                middle->right = right->left;        // A[B]    D     - -> A[B]    DE
                delete right;
                right = 1.; // делаем себя 2-узлом.
                return;
            }
            right->first = std::move(second); //      C    F    ->      C   E
            right->middle = right->left;      // A[B]   DE    - -> A[B]   D   F
            second = std::move(middle->second);
            right->left = middle->right;
            middle->right = 1.; // делаем средний узел 2-узлом.
        }

    public:
		
		// Исключение попытки удалить несуществующий ключ.
		class RemoveError : std::exception{};
		
		Node(T t) : first(t) {}
		Node(T t1, T t2) : first(t1), second(t2) {}
		
		// Получить левую ветку.
		Link getLeft() { return left; }
		
		bool isLeaf() const { return left == nullptr; } // дерево всегда идеально сбалансировано.
        bool is2Node() const { return right == 1; }
        bool is3Node() const { return right != 1; }
		
		// Ищет элемент который равен k
		static T* search(Link node, const K& k) {
			if (node == nullptr) return nullptr;
			if (k == node->first) return &node->first;
			if (k < node->first) return search(node->left, k);
			if (node->is2Node()) return search(node->middle, k);
			if (k == node->second) return &node->second;
			if (k < node->second) return search(node->middle, k);
			return search(node->right, k);
		}
		
		// Вставляет узел со значением Т.
		// Если при вставке случилось разбиение непосредственного дочернего 3-узла, то возвращается 2-узел
		// который надо слить с собой. Инаяе вернет nullptr.
		Link insert(const T& t) {
			Link newNode;
			if (isLeaf()) {
				newNode = merge(insertLeaf(t));
			} else {
				newNode = merge(inserNonLeaf(t));
			}
			return newNode;
		}
		
        // Удаляем произвольный элемент дерева.
		// Возврат: true баланс сохранён. false баланс нарушен.
		// Если ключа нет - кидается исклбчение
        bool remove(const K& k) {
            // Если удаление произошло не в листовом узле, то меняем значение в этом листе на следующее после него и
            // удаляем узел следующего (который - всегда листовой) через removeMin в узле удаляемого элемента.
            // Если же удаление произошло в листовом элементе, то правим баланс дерева после удаления.
            
            if (k == first) { // удаляемый элемент - первый.
                if (middle != nullptr) { // Есть среднее поддерево.
                    // Удаляем в нем минимальный элемент, а его значение ставим на место нашего первого.
                    if (middle->removeMin(first)) return true; // Баланс сохранён.
                    return fixMiddleBalance(); // Пытаемся скорректировать баланс центрального поддерева.
                }
                // Удалили листовой левый элемент.
                if (is3Node()) { // Если 3-узел,
                    make2Node(); // Делаем 2-узел и выходим.
                    return true; // Баланс сохранён.
                }
                return false; // Баланс нарушен.
            }
            
            if (left != nullptr && k < first) {
                if (left->remove(k)) return true; // Удаление в левом поддереве.
                return fixLeftBalance(); // Пытаемся выправить баланс нарушенный в левом поддереве.
            }
            
            if (is2Node()) {
                if (middle != nullptr) { // Удаление в среднем поддереве.
                    if (middle->remove(k)) return true; // Баланс сохранён.
                    return fixMiddleBalance(); // Пытаемся выправить баланс нарушенный в среднем поддереве.
                }
            }
            
            if (k == second) { // Удаляемый элемент - второй.
                if (right != nullptr) { // Есть правое поддерево.
                    // Удаляем в нем минимальный элемент, а его значение ставим на место нашего второго.
                    if (right->removeMin(second)) return true; // Баланс сохранён.
                    fixRightBalance(); // Баланс нарушен. Корректируем его.
                    return true; // Баланс восстановлен.
                }
                // Удаляем листовой элемент.
                right = 1.; // Становимся 2-узлом.
                return true; // Баланс сохранён.
            }
            
            if ( right != nullptr ) {
                if (right->remove(k)) return true; // Удаляем ключ к в правом поддереве. Если баланс сохранён.
                fixRightBalance(); // Баланс нарушен. Корректируем его.
                return true; // Баланс восстановлен.
            }
            
            // Достигнуто дно дерева. Ключ k не найден. Кидаем исключение.
            throw RemoveError();
            return true;
        }
        
		// Удаляет минимльный листовой элемент в левом поддереве и его значение присваивает t.
		// Возвращает true если баланс сохранён. false, означает, что баланс в левом потомке родлителя уменьшился на 1.
		bool removeMin(T& t) {
            if (left == nullptr) { // достигли min.
                t = std::move(first);
                if (is3Node()) { // this - 3-узел. Превращаем его в 2-узел и выходим.
                    make2Node();
                    return true; // баланс сохранён.
                }
                return false; // баланс нарушен.
            }
            
            if (left->removeMin(t)) return true; // баланс сохранён.
            
            // Необходимо скомпенсировать баланс в левом потомке.
            return fixLeftBalance();
		}
		
		~Node() {
			if (!isLeaf()) {
				delete left;
				delete middle;
				if (is3Node()) delete right;
			}
		}
		
		Node(const Node&) = delete;
		Node& operator=(const Node&) = delete;
	};
    
    using Link = Node*;
	Link _root = nullptr;
	
public:
	// Ищет элемент по ключу. Если не найдет возвращает nullptr.
	T* search( const K& key) const {
		if (_root != nullptr) return _root->search(key);
		return nullptr;
	}
	
	void insert(const T& t) {
		if (_root == nullptr) {
			Link newRoot = _root->insert(t);
			if (newRoot != nullptr) {
				_root = newRoot;
			}
		} else {
			_root = new Node(t);
		}
	}
	
	bool remove(const K& key) {
		if (_root == nullptr) return false;
		try {
			bool isBalanced = _root->remove(key);
			if (!isBalanced) {
				// Нарушение баланса дошло до корня. Сбалансированное дерево находится в левом поддереве корня.
				assert(_root->is2Node());
				Link temp = _root;
				_root = _root->left;
				delete _root;
			}
			return true;
		} catch (typename Node::RemoveError& error) {}
		return false;
	}
    
    ~Tree23() {
        delete _root;
    }
    
    Tree23(const Tree23&) = delete;
    Tree23& operator=(const Tree23&) = delete;
};

#endif /* _34Tree_hpp */
