#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <binary_search.c>

void sort_fill(int size, int arr[size]) {
    srand(time(0));

    arr[0] = 1;
    int i = 1;
    while (i < size) {
        int temp = rand() % (i * 10);
        if (temp > arr[i - 1])
            arr[i++] = temp;
    }
}

void print(int size, int arr[size]) {
    for (int i = 0 ; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}


int main()
{
    int arr[SIZE] = { 0 };
    
    sort_fill(SIZE, arr);
    print(SIZE, arr);

    int value;
    printf("Enter value to search: ");
    if (!scanf("%d", &value))
        exit(1);
    
    printf("%s", search(value, SIZE, arr) ? "Found" : "Not found");
    
    return 0;
}
