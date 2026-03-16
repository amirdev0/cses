#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20

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
    if (left(index) > size || right(index) > size)
        return;

    if (arr[index] < arr[left(index)]) {
        swap(&arr[index], &arr[left(index)]);
        max_heapify(left(index), size, arr);
    }
    
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
    for (int i = 0; i < size; i++)
        build_max_heap(size - i, arr + i);
}

int main()
{
    int arr[SIZE] = { 0 };
    
    rand_fill(SIZE, arr);
    print(SIZE, arr);
    
    heapsort(SIZE, arr);
    print(SIZE, arr);
    
    return 0;
}
