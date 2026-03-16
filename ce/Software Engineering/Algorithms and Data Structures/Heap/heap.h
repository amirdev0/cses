#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 12

struct heap {
    int size;
    int *arr;
    int height;
};

void rand_fill(int size, int arr[size]) {
    srand(time(0));
    for (int i = 0; i < size; i++)
        arr[i] = rand() % 100;
}

void print(int size, int arr[size]) {
    for (int i = 0 ; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int parent(int index) { return (index - 1) / 2; }
int left(int index) { return 2 * index + 1; }
int right(int index) { return 2 * index + 2; }

void max_heapify(int index, int size, int arr[size]) {
    if (left(index) >= size)
        return;
    
    if (arr[index] < arr[left(index)]) {
        swap(&arr[index], &arr[left(index)]);
        max_heapify(left(index), size, arr);
    }
    
    if (right(index) >= size)
        return;
    
    if (arr[index] < arr[right(index)]) {
        swap(&arr[index], &arr[right(index)]);
        max_heapify(right(index), size, arr);
    }
}

void build_max_heap(int size, int arr[size]) {
    for (int i = size / 2 - 1; i >= 0; i--)
        max_heapify(i, size, arr);
}

void heapsort(int size, int arr[size]) {
    for (int i = 0; i < size; i++) {
        build_max_heap(size - i, arr);
        swap(&arr[size - 1 - i], &arr[0]);
    }
}

int heap_maximum(int size, int arr[size]) {
    build_max_heap(size, arr);
    return arr[0];
}

int heap_extract_max(int *size, int arr[*size]) {
    if (*size < 1)
        return -1;
    
    build_max_heap(*size, arr);
    swap(&arr[*size - 1], &arr[0]);
    build_max_heap(*size - 1, arr);
    *size -= 1;
    
    return arr[*size];
}

void max_heap_insert(int value, int *size, int arr[*size]) {
    arr[*size] = value;
    *size += 1;
    build_max_heap(*size, arr);
}

void heap_increase_key(int value, int index, int size, int arr[size]) {
    if (index < 0 || index >= size)
        return;
    
    if (key < arr[index])
        return;
    
    arr[index] = value;
    build_max_heap(size, arr);
}

int main()
{
    int size = SIZE - 10;
    int arr[SIZE] = { 0 };
    
    rand_fill(size, arr);
    print(size, arr);
    
    heapsort(size, arr);
    print(size, arr);
    
    max_heap_insert(77, &size, arr);
    print(size, arr);
    
    
    return 0;
}


