#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

bool isSorted(int arr[], size_t length);
void insertionSort(int arr[], size_t lowerBound, size_t upperBound);
void merge(int arr[], size_t lowerBound, size_t midPoint, size_t upperBound);
size_t min(size_t a, size_t b);
void timSort(int arr[], size_t arr_length);