#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 10

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int arr[SIZE];
    srand(time(0));
    for (int i = 0; i < SIZE; i++)
        arr[i] = rand() % 100;
    
    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE - i - 1; j++) {
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
        }
        
    }
    
    
    for (int i = 0; i < SIZE; i++)
        printf("%d ", arr[i]);
    printf("\n");
    return 0;
}

