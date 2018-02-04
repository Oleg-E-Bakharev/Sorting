#include "radixSort.hpp"
#include <climits>
#include "testSort.hpp"
#include <iostream>
#include <assert.h>
#include "common.h"
#include "rnd.h"
#include <chrono>

using namespace std;
using namespace chrono;

static const int BitsDigit = 8;
static const int BitsByte = 8;
static const int BytesWord = sizeof(int) * BitsByte / BitsDigit;
static const int CountSize = (1 << BitsDigit) + 1;

static inline int digit(int i, int d)
{
    return (i >> (BitsDigit * d)) & ((1 << BitsDigit) - 1);
}

void radixSort(int* a, int* b, int n, unsigned* count)
{
    for (int d = 0; d < BytesWord; d++) {
        for (int j = 0; j < CountSize; j++) count[j] = 0;
        for (int i = 0; i < n; i++) count[digit(a[i], d) + 1]++;
        for (int j = 1; j <= CountSize; j++) count[j] += count[j-1];
        for (int i = 0; i < n; i++) {
            int dig = digit(a[i], d);
            int bPos = count[dig]++;
            b[bPos] = a[i];
        }
        for (int i = 0; i < n; i++) a[i] = b[i];
    }
}

// Рекурсивная реализация.
void radixSort2(int d, int* a, int* b, int n, unsigned* count)
{
    if (d == BytesWord) {
        return;
    }
    for (int j = 0; j < CountSize; j++) count[j] = 0;
    for (int i = 0; i < n; i++) count[digit(a[i], d) + 1]++;
    for (int j = 1; j <= CountSize; j++) count[j] += count[j-1];
    for (int i = 0; i < n; i++) {
        int dig = digit(a[i], d);
        int bPos = count[dig]++;
        b[bPos] = a[i];
    }
    radixSort2(d + 1, b, a, n, count);
}

void callRadix(int* a, int n)
{
    int* b = new int[n];
    unsigned* count = new unsigned[CountSize];
    
    radixSort(a, b, n, count);
    randomizeIntArray(a, n);
    radixSort2(0, a, b, n, count);
    
    delete [] b;
    delete [] count;
}

void testRadix(int *a, int n)
{
    int* b = new int[n];
    
    unsigned* count = new unsigned[CountSize];

	auto start = high_resolution_clock::now();
    radixSort(a, b, n, count);
	auto performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
	std::cout << "radixSort: " << performance << endl;

    testSort(a, n);

    randomizeIntArray(a, n);
    
	start = high_resolution_clock::now();
    radixSort2(0, a, b, n, count);
	performance = duration_cast<microseconds>( high_resolution_clock::now() - start ).count();
    cout << "radixSort2: " << performance <<endl;

    testSort(a, n);
    
    
    delete [] b;
    delete [] count;
}
