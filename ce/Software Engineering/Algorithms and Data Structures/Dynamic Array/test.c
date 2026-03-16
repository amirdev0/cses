#include <stdio.h>
#include <dynamic_array.h>

void fill(int value, struct array *arr) {
    while (arr->size < arr->capacity)
        push_back(value, arr);
}

void rand_fill(struct array *arr) {
    srand(time(0));
    while (arr->size < arr->capacity)
        push_back(rand() % 100, arr);
}

void test(struct array *arr) {
    puts("Test of dynamic array\n");
    printf("Popped value: %d\n", pop_back(arr));
    printf("Popped value: %d\n", pop_back(arr));
    printf("Remove at index %d: %d\n", 0, remove_at(0, arr));
    printf("Remove at index %d: %d\n", 21, remove_at(21, arr));
    printf("Push %d\n", remove_at(0, arr));
}

int main() {
    struct array *arr = calloc(1, sizeof(struct array));

    test(arr);

    int choice = -1;
    while (choice) {
        puts("\n--- Dynamic Array ---");
        puts("1. Push value to the back");
        puts("2. Pop value from the back");
        puts("3. Insert value at index");
        puts("4. Remove value at index");
        puts("5. Assign value at index");
        puts("6. Search value in array");
        puts("7. Reserve space for array");
        puts("8. Fill/Clear array");
        puts("9. Print array iformation");
        puts("0. Exit");
        
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) == 0)
            break;
        
        int index = -1, value = 0;
        switch (choice) {
            case 1:
                printf("Enter value to push: ");
                scanf("%d", &value);
                push_back(value, arr);
                break;
            case 2:
                value = pop_back(arr);
                printf("Popped value: %d\n", value);
                break;
            case 3:
                printf("Enter index: ");
                scanf("%d", &index);
                printf("Enter value to insert: ");
                scanf("%d", &value);
                insert_at(index, value, arr);
                break;
            case 4:
                printf("Enter index: ");
                scanf("%d", &index);
                printf("Enter value to remove: ");
                scanf("%d", &value);
                remove_at(index, arr);
                break;
            case 5:
                printf("Enter index: ");
                scanf("%d", &index);
                printf("Enter value to assign: ");
                scanf("%d", &value);
                assign_at(index, value, arr);
                break;
            case 6:
                printf("Enter value to search: ");
                scanf("%d", &value);
                if ((index = search(value, arr)) == -1)
                    printf("Not found\n");
                else
                    printf("Found at index %d\n", index);
                break;
            case 7:
            {
                int space;
                printf("Enter space to reserve: ");
                scanf("%d", &space);
                if (reserve(space, arr))
                    puts("Succeed to reserve space");
                else
                    puts("Failed to reserve space");
                break;
            }
            case 8:
            {
                int operation;
                puts("\n1. Fill array with designated value");
                puts("2. Fill array with random values");
                puts("3. Clear array (set values to zero)");
                
                printf("\nEnter number of operation: ");
                if (scanf("%d", &operation) == 0)
                    break;
                
                if (operation == 1) {
                    printf("Enter value to fill array: ");
                    scanf("%d", &value);
                    fill(value, arr);
                }
                if (operation == 2)
                    rand_fill(arr);
                if (operation == 3)
                    clear(arr);
                break;
            }
            case 9:
                printf("Array capacity = %d, size = %d\n", capacity(arr), size(arr));
                for (int i = 0; i < arr->size; i++)
                    printf("%d ", at(i, arr));
                printf("\n");
                break;
        }
    }

    free(arr);    
    return 0;
}
