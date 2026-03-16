#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

struct list {
    struct node *head;
};

struct node* find_prev(struct node *node, struct list *list) {
    if (list->head == NULL)
        return NULL;
    
    struct node *prev, *curr = list->head;
    do {
        prev = curr;
        curr = curr->next;
        if (curr == node)
            return prev;
    } while (curr != list->head);

    return NULL;
}

struct node* search(int value, struct list *list) {
    if (list->head == NULL)
        return NULL;
    
    struct node *node = list->head;
    do {
        if (node->value == value)
            return node;
        node = node->next;
    } while (node != list->head);
    
    return NULL;
}

int insert(int value, struct list *list) {
    if (list->head == NULL) {
        list->head = malloc(sizeof(struct node));
        list->head->value = value;
        list->head->next = list->head;
        
        return 0;
    }
    
    struct node *new_node = malloc(sizeof(struct node));
    new_node->value = value;
    new_node->next = list->head;
    
    struct node *prev = find_prev(list->head, list);
    prev->next = new_node;

    return 0;
}

int delete(int value, struct list *list) {
    if (list->head == NULL)
        return -1;
    
    struct node *node = search(value, list);
    if (node == NULL)
        return -1;
    
    struct node *prev = find_prev(node, list);
    prev->next = node->next;

    if (list->head->next == list->head)
        list->head = NULL;
    
    if (node == list->head)
        list->head = prev;

    free(node);
    return 0;
}

void draw(struct list *list) {
    if (list->head == NULL)
        return;

    struct node *node = list->head;
    do {
        printf("%d -> ", node->value);
        node = node->next;
    } while (node != list->head);
    puts("x");
}

void test(struct list *list) {
    puts("Circular Linked List test\n");
    
    printf("search 5: %s\n", search(5, list) != NULL ? "succeed" : "failed");
    
    printf("delete 5: %s\n", delete(5, list) == 0 ? "succed" : "failed");
    printf("insert 5: %s\n", insert(5, list) == 0 ? "succeed" : "failed");
    printf("insert 3: %s\n", insert(3, list) == 0 ? "succeed" : "failed");
    
    puts("\nList structure:");
    draw(list);
    
    printf("search 5: %s\n", search(5, list) != NULL ? "succeed" : "failed");
    printf("delete 5: %s\n", delete(5, list) == 0 ? "succed" : "failed");
    
    puts("\nList structure:");
    draw(list);
    
    printf("search 5: %s\n", search(5, list) != NULL ? "succeed" : "failed");
    printf("delete 3: %s\n", delete(3, list) == 0 ? "succed" : "failed");
    
    puts("\nList structure:");
    draw(list);
}

int main()
{
    struct list *list = calloc(1, sizeof(struct list));

    test(list);
    
    int choice = -1;
    while (choice) {
        puts("\n1. Insert value to circular linked list");
        puts("2. Delete value from circular linked list");
        puts("3. Search value in circular linked list");
        puts("4. Draw graphically circular linked list");
        puts("0. Exit");
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        
        int value;
        switch (choice) {
            case 1:
                printf("Enter value to be inserted: ");
                scanf("%d", &value);
                if (insert(value, list) == 0)
                    puts("Suceed to insert node");
                else
                    puts("Failed to insert node");
                break;
            case 2:
                printf("Enter value to be deleted: ");
                scanf("%d", &value);
                if (delete(value, list) == 0)
                    puts("Suceed to delete node");
                else
                    puts("Failed to delete node");
                break;
            case 3:
                printf("Enter value to search for: ");
                scanf("%d", &value);
                if (search(value, list) != 0)
                    puts("Node was found");
                else
                    puts("Node was not found: ");
                break;
            case 4:
                puts("List structure");
                draw(list);
                break;
        }
    }
    
    free(list);
    return 0;
}

