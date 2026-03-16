#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5

struct node {
    int value;
    struct node *next;
};

void print(int size, struct node *hash_table[size]) {
    for (int i = 0 ; i < size; i++) {
        struct node *node = hash_table[i];
        while (node) {
            printf("%d -> ", node->value);
            node = node->next;
        }
        printf("x\n");
    }
}

int hash(int key) { return key % 5; }

void add(int value, int size, struct node *hash_table[size]) {
    int index = hash(value);

    if (hash_table[index] == NULL) {
        hash_table[index] = malloc(sizeof(struct node));
        hash_table[index]->value = value;
        hash_table[index]->next = NULL;
        
        return;
    }
    
    struct node *prev, *node = hash_table[index];
    while (node) {
        prev = node;
        node = node->next;
    }
    
    node = malloc(sizeof(struct node));
    node->value = value;
    node->next = NULL;
    prev->next = node;
}

int main()
{
    int size = SIZE;
    struct node *hash_table[SIZE] = { NULL };
    
    add(1, size, hash_table);
    add(6, size, hash_table);
    add(43, size, hash_table);
    add(21, size, hash_table);
    add(27, size, hash_table);
    print(size, hash_table);
    
    return 0;
}


