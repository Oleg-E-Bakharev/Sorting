//
//  SplayTree.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 25.09.16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "SplayTree.hpp"
#include <random>

void testSplayTree()
{
    using namespace std;
    random_device rd;
    auto random = default_random_engine(rd());
    
    //char C[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";//АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";
    char C[] = "0123456789";//ABCDEFGHIJKLMNOPQRSTUVWXYZ";//abcdefghijklmnopqrstuvwxyz";
    
    SplayTree<char> tree;
    tree.insert(begin(C), end(C)-1);
    auto t = tree.info();
    cout << t << "\n================\n";
    
    tree.search('0');
    cout << t << "\n================\n";

    tree.search('1');
    cout << t << "\n================\n";
    
    tree.search('2');
    cout << t << "\n================\n";
	
	tree.remove('7');
	cout << t << "\n================\n";
	tree.remove('7');
	cout << t << "\n================\n";
	tree.remove('6');
	cout << t << "\n================\n";
	tree.remove('5');
	cout << t << "\n================\n";
}
