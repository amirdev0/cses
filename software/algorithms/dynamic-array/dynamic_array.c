void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int* resize(int new_capacity, struct array *arr) {
    if (arr->begin = realloc(arr->begin, new_capacity * sizeof(int)))
        arr->capacity = new_capacity;
    
    return arr->begin;
}

int expandable(struct array *arr) {
    return arr->size + 1 > arr->capacity;
}

int shrinkable(struct array *arr) {
    return arr->size <= arr->capacity / 2;
}


int size(struct array *arr) {
    return arr->size;
}

int capacity(struct array *arr) {
    return arr->capacity;
}

void push_back(int value, struct array *arr) {
    if (arr->begin == NULL) {
        arr->begin = arr->end = malloc(sizeof(int));
        arr->capacity = 1;
    }

    if (expandable(arr)) {
        arr->begin = resize(arr->capacity * 2, arr);
        arr->end = arr->begin + arr->size;
    }
    
    *arr->end++ = value;
    arr->size++;
}

int pop_back(struct array *arr) {
    if (arr->size == 0) {
        if (arr->begin != NULL)
            free(arr->begin);
        
        arr->begin = arr->end = NULL;
        arr->capacity = 0;
        
        return -1;
    }

    int value = *(arr->end -= 1);
    arr->size--;

    if (shrinkable(arr)) {
        arr->begin = resize(arr->capacity / 2, arr);
        arr->end = arr->begin + arr->size;
    }

    return value;
}

int at(int index, struct array *arr) {
    if (index > arr->size - 1 || index < 0)
        return -1;
    
    return arr->begin[index];
}

int assign_at(int index, int value, struct array *arr) {
    if (at(index, arr) == -1)
        return -1;

    arr->begin[index] = value;
    
    return 0;
}

int insert_at(int index, int value, struct array *arr) {
    if (at(index, arr) == -1)
        return -1;

    push_back(value, arr);
    
    for (int i = arr->size - 2; i >= index; i--)
        swap(&arr->begin[i + 1], &arr->begin[i]);
    
    return 0;
}

int remove_at(int index, struct array *arr) {
    if (at(index, arr) == -1)
        return -1;
    
    for (int i = index; i < arr->size - 1; i++)
        swap(&arr->begin[i], &arr->begin[i + 1]);
    
    pop_back(arr);
    
    return 0;
}

int search(int value, struct array *arr) {
    for (int i = 0; i < arr->size; i++)
        if (arr->begin[i] == value)
            return i;

    return -1;
}

int reserve(int size, struct array *arr) {
    int *result;
    if (size > arr->size) 
        result = resize(size, arr);
    arr->end = arr->begin + arr->size;
    
    return result != NULL;
}

void clear(struct array *arr) {
    for (int i = 0; i < arr->size; i++)
        arr->begin[i] = 0;
}
