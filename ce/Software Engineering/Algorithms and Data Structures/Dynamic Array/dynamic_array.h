#include <stdlib.h>

struct array {
    int *begin;
    int *end;
    int size;
    int capacity;
};

void swap(int *a, int *b);

int* resize(int new_capacity, struct array *arr);

int expandable(struct array *arr);

int shrinkable(struct array *arr);

int size(struct array *arr);

int capacity(struct array *arr);

void push_back(int value, struct array *arr);

int pop_back(struct array *arr);

int at(int index, struct array *arr);

int assign_at(int index, int value, struct array *arr);

int insert_at(int index, int value, struct array *arr);

int remove_at(int index, struct array *arr);

int search(int value, struct array *arr);

int reserve(int size, struct array *arr);

void clear(struct array *arr);

