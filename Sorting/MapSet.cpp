//
//  MapSet.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 20.11.2017.
//  Copyright © 2017 Oleg Bakharev. All rights reserved.
//

#include "MapSet.hpp"
#include <string>
#include <assert.h>

using namespace std;

void testMapSet()
{
	Set<int> s {3, 10, 5, 1};
	assert(s.has(5) && !s.has(6));
	assert(s.size() == 4);
	for( int i : s) { cout << i << ", "; } cout << "\n";
	
    Map<string, int> m = {{"Kuznetsov", 6}, {"Ivanov", 3}, {"Petrov", 2}, {"Sidorov", 5}};
    m.insert("Lomonosov", 9);
    
    // Проверка что [] с отсутствующим ключом не приводит к вставке элемента.
    auto refPopov = m["Popov"]; // // Map<string, int>::reference
    assert(m.has("Ivanov") && !m.has("Popov"));
    for (auto i : m) { cout << "{" << i.first << ", " << i.second << "}, "; } cout << "\n";
    
    // Проверка что добалвение элемента ранее отсутвующего но с имеющейся ссылкой добавляет элемент и в ссылку.
    m["Popov"] = 6;
    cout << "Popov = " << refPopov << "\n";
    
    // Проверка изменения в ссылке.
    auto refIvanov = m["Ivanov"];
    cout << "Ivanov = " << refIvanov << "\n";
    m["Ivanov"] = 10;
    cout << "Ivanov = " << refIvanov << "\n";
    refIvanov = 11;
    cout << "Ivanov = " << refIvanov << "\n";
    cout << "Ivanov = " << m["Ivanov"] << "\n";
	
	int& i = refIvanov;
	i++;
    
    // Проверка на взятие медианы.
    //cout << "Pushkin = " << m["Pushkin"] << "\n";
    m["Lermontov"] = 20;
    for (auto i : m) { cout << "{" << i.first << ", " << i.second << "}, "; } cout << "\n";
    auto median = m.begin()[m.size() / 2]; // Map<string, int>::iterator
    cout << "Median = " << median.first << ", " << median.second << "\n";
}
