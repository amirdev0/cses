#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 12

struct heap {
    int size;
    int *arr;
    int height;
};

void max_heapify(int index, int size, int arr[size]);

void build_max_heap(int size, int arr[size]);

void heapsort(int size, int arr[size]);

int heap_maximum(int size, int arr[size]);

int heap_extract_max(int *size, int arr[*size]);

void max_heap_insert(int value, int *size, int arr[*size]);

