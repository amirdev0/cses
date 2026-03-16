#include <stdio.h>
#include <stdlib.h>

#define SIZE 1024

struct stack {
    int elements[SIZE];
    int *top;
};

void push(int value, struct stack *stack) {
    if (stack->top > stack->elements)
        *--stack->top = value;
}

int pop(struct stack *stack) {
    int value = -1;
    if (stack->top < stack->elements + SIZE)
        value = *stack->top++;

    return value;
}

int peek(struct stack *stack) {
    int value = pop(stack);
    if (value != -1)
        push(value, stack);

    return value;
}

