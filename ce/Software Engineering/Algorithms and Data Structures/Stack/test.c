
void test(struct stack *stack) {
    printf("peek:\t%d\n", peek(stack));
    printf("pop:\t%d\n", pop(stack));
    printf("push:\t3\n"), push(3, stack);
    printf("peek:\t%d\n", peek(stack));
    printf("push:\t6\n"), push(6, stack);
    printf("pop:\t%d\n", pop(stack));
    printf("push:\t5\n"), push(5, stack);
    printf("pop:\t%d\n", pop(stack));
    printf("pop:\t%d\n", pop(stack));
    printf("pop:\t%d\n", pop(stack));
}

int main()
{
    struct stack *stack = malloc(sizeof(struct stack));
    stack->top = stack->elements + SIZE;
    
    //test(stack); return 0;
    
    int choice = -1;
    while (choice) {
        puts("\n1. Push value to stack");
        puts("2. Pop value from stack");
        puts("3. Peek stack");
        puts("0. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        switch (choice) {
        case 1:
        {
            int value;
            printf("Enter value to be pushed: ");
            scanf("%d", &value);
            push(value, stack);
            if (peek(stack) == value)
                puts("Push succeed");
            else
                puts("Push failed");
            break;
        }
        case 2:
        {
            int value = pop(stack);
            if (value != -1)
                printf("Popped value: %d\n", value);
            else
                printf("Stack is empty\n");
            break;
        }
        case 3:
        {
            int value = peek(stack);
            if (value != -1)
                printf("Peeked value: %d\n", value);
            else
                printf("Stack is empty\n");
            break;
        }
        }
    }
    
    free(stack);
    
    return 0;
}
