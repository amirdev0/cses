#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 40

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void qksort(int *arr, int *low, int *high) {
    if (high <= low)
        return;
    
    int *beg = low;
    int *end = high - 1;
    
    int *key = high;
    while (beg < end) {
        if (*beg >= *key) {
            if (*end < *key) {
                swap(beg, end);
                beg++; end--;
            }
            else
                end--;
        }
        else
            beg++;
    }
    
    if (*key >= *end)
        swap(key, ++end);
    else
        swap(high, end++);

    qksort(arr, low, beg);
    qksort(arr, end, high);
}

void fill_rand(int *arr) {
    srand(time(0));
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100;
}

void print(int *arr) {
    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main()
{
    int arr[SIZE];
    
    fill_rand(arr);
    print(arr);
    qksort(arr, arr, arr + SIZE - 1);
    print(arr);

    return 0;
}
