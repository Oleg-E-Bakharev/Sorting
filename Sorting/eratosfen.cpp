//
//  eratosfen.cpp
//  Sorting
//
//  Created by Oleg Bakharev on 10/08/16.
//  Copyright © 2016 Oleg Bakharev. All rights reserved.
//

#include "eratosfen.hpp"
#include <iostream>
#include "stdio.h"

using namespace std;

// Заносит в массив primes простые числа до значени max
size_t eratosfen(size_t max, size_t* primes, size_t maxPrimes)
{
	bool compound[1000] = {0};
	size_t count = 0;
	size_t current = 2;
	for (;current < max; ++current) {
		if (!compound[current]) {
			if (count == maxPrimes) {
				return -1; // не хватило размера выходного массива для простых чисел.
			}
			primes[count] = current;
			count++;
			// Заполнение решета с проверкой переполнения.
			for (size_t i = current * current; i < max; i += current) {
				compound[i] = true;
			}
		}
	}
	return count;
}

int testEratosfen(size_t n)
{
//	const size_t max = 1000;
//	const size_t maxPrimes = 200;
//	size_t primes[maxPrimes];
//	size_t count = eratosfen(max, primes, maxPrimes);
//	if (count == -1) {
//		printf("Not enough prime numbers buffer size\n");
//		return -1;
//	}
//	printf("Eratosfen to %d counts %d prime numbers\n", max, count);
//	for (size_t i = 0; i < count; ++i) {
//		printf("%d\n", primes[i]);
//	}
	
	cout << boolalpha;
	Eratosfen e(1000);
	cout << e;
	cout << "is 53 a prime number: " << e.isPrime(53) << "\n";
	cout << "is 777 a prime number: " << e.isPrime(777) << "\n";
	cout << "16th prime number is: " << e[15] << "\n";
	
//    PrimeNumbersGenerator g(n);
//    while(g) { std::cout << g << std::endl; }
//    std::cout << g;
//    std::cout << "|\n";
//    std::cout << g;
//    while(g) { std::cout << g << std::endl; }
	return 0;
}
