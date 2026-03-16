#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//For debugging
#define STR(x) #x
#define NODE(x) printf(STR(x)": %p\n", (x) ? (x) : NULL)
#define VALUE(x) printf(STR(x)"->value: %d\n", (x) ? (x)->value : -1)
#define LEFT(x) printf(STR(x)"->left: %p\n", (x) ? (x)->left : NULL)
#define RIGHT(x) printf(STR(x)"->right: %p\n", (x) ? (x)->right : NULL)


struct node {
    int value;
    struct node *left;
    struct node *right;
};

struct tree {
    struct node *root;
    int height;
};

int max(int a, int b) {
    return a >= b ? a : b;
}

struct node* search(int value, struct tree *tree) {
    if (tree == NULL)
        return NULL;
    
    struct node *node = tree->root;
    while (node) {
        if (value == node->value)
            break;

        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }
    
    return node;
}

int height(struct tree *tree) {
    if (tree == NULL)
        return -1;

    if (tree->root == NULL)
        return 0;

    return max(height(&(struct tree){ .root = tree->root->left }), height(&(struct tree){ .root = tree->root->right })) + 1;
}

struct node* find_parent_node(int value, struct tree *tree) {
    if (tree == NULL)
        return NULL;
    
    struct node *key = search(value, tree);
    if (key == NULL)
        return NULL;
    
    if (key == tree->root)
        return NULL;
    
    struct node *prev, *node = tree->root;
    while (node) {
        if (node == key)
            break;
        
        prev = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }
    
    return prev;
}

struct node* find_predecessor(int value, struct tree *tree) {
    if (tree == NULL)
        return NULL;
    
    struct node *key = search(value, tree);
    if (key == NULL)
        return NULL;
        
    if (key->left == NULL)
        return NULL;
    
    struct node *prev, *node = key->left;
    while (node) {
        prev = node;
        node = node->right;
    }
    
    return prev;
}

struct node* find_successor(int value, struct tree *tree) {
    if (tree == NULL)
        return NULL;
    
    struct node *key = search(value, tree);
    if (key == NULL)
        return NULL;
        
    if (key->right == NULL)
        return NULL;
    
    struct node *prev, *node = key->right;
    while (node) {
        prev = node;
        node = node->left;
    }
    
    return prev;
}

struct node* min_node(struct tree *tree) {
    if (tree == NULL)
        return NULL;

    if (tree->root == NULL)
        return NULL;

    struct node *prev, *node = tree->root;
    while (node) {
        prev = node;
        node = node->left;
    }
    
    return prev;
}

struct node* max_node(struct tree *tree) {
    if (tree == NULL)
        return NULL;

    if (tree->root == NULL)
        return NULL;

    struct node *prev, *node = tree->root;
    while (node) {
        prev = node;
        node = node->right;
    }
    
    return prev;
}

int insert(int value, struct tree *tree) {
    if (tree == NULL)
        return -1;

    if (tree->root == NULL) {
        tree->root = (struct node *)malloc(sizeof (struct node));
        if (tree->root == NULL) {
            fprintf(stderr, "Error in memory allocation\n");
            exit(EXIT_FAILURE);
        }
    
        tree->root->value = value;
        tree->height = 1;
        return 0;
    }

    struct node *key = search(value, tree);
    if (key != NULL)
        return -1;
    
    struct node *prev, *node = tree->root;
    while (node) {
        prev = node;
        if (value < node->value)
            node = node->left;
        else
            node = node->right;
    }

    struct node *temp = (struct node *)malloc(sizeof (struct node));
    if (temp == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    
    temp->value = value;
    temp->left = temp->right = NULL;
    if (value < prev->value)
        prev->left = temp;
    else
        prev->right = temp;

    return 0;
}

int delete(int value, struct tree *tree) {
    if (tree == NULL)
        return -1;
    
    struct node *key = search(value, tree);
    if (key == NULL)
        return -1;
    
    struct node *replacement = NULL;
    if (key->left)
        replacement = find_predecessor(value, tree);
    if (key->right) 
        replacement = find_successor(value, tree);

    if (replacement) {
        struct node *replacement_parent = find_parent_node(replacement->value, tree);
        if (replacement_parent->left == replacement)
            replacement_parent->left = (key->right ? replacement->right : replacement->left);
        else
            replacement_parent->right = (key->right ? replacement->right : replacement->left);
        
        replacement->left = key->left;
        replacement->right = key->right;
    }
    
    struct node *parent = find_parent_node(value, tree);
    if (parent)
        if (parent->left == key)
            parent->left = replacement;
        else
            parent->right = replacement;

    if (key == tree->root)
        tree->root = replacement;
    
    free(key);
    return 0;
}

void inorder(struct tree *tree) {
    if (tree == NULL)
        return;
        
    struct node *node = tree->root;
    if (node) {
        printf("%d ", node->value);
        fflush(stdout);
        
        tree->root = node->left;
        inorder(tree);

        tree->root = node->right;
        inorder(tree);
        
        tree->root = node;
    }
}

void preorder(struct tree *tree) {
    if (tree == NULL)
        return;
        
    struct node *node = tree->root;
    if (node) {
        tree->root = node->left;
        preorder(tree);

        printf("%d ", node->value);
        fflush(stdout);

        tree->root = node->right;
        preorder(tree);
        
        tree->root = node;
    }
}

void postorder(struct tree *tree) {
    if (tree == NULL)
        return;
        
    struct node *node = tree->root;
    if (node) {
        tree->root = node->left;
        postorder(tree);

        tree->root = node->right;
        postorder(tree);
        
        printf("%d ", node->value);
        fflush(stdout);
        
        tree->root = node;
    }
}

void draw(struct tree *tree) {
    if (tree == NULL)
        return;

    if (tree->root == NULL)
        return;
    
    int capacity = (1 << height(tree)) - 1;
    int *values = (int *)malloc(capacity * sizeof(int));
    if (values == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    
    values[0] = tree->root->value;
    for (int i = 0, j = 1; j < capacity; i++) {
        struct node *node = search(values[i], tree);
        
        if (node == NULL) {
            values[j++] = -1;
            values[j++] = -1;
            continue;
        }
        
        if (node->left) values[j++] = node->left->value;
        else values[j++] = -1;
            
        if (node->right) values[j++] = node->right->value;
        else values[j++] = -1;
    }
    
    int tree_height = height(tree);
    int current_level = 0;
    for (int i = 0; i < capacity; i++) {
        if (i + 1 > ((1 << current_level) - 1))
            printf("\n%*c", (1 << tree_height - current_level++) - 1, ' ');
        
        int width = (1 << (tree_height - current_level) + 2);
        if (values[i] == -1)
            printf("%-*s", width, "xx");
        else
            printf("%-*.2d", width, values[i]);
    }
    
    printf("\n");
    free(values);
}

void test(struct tree *tree) {
    puts("Binary Search Tree test");
    
    insert(60, tree);
    insert(40, tree);
    insert(80, tree);
    insert(30, tree);
    insert(50, tree);
    insert(70, tree);
    insert(90, tree);
    puts("\n\nInitial tree");
    draw(tree);
    
    puts("\n\nDelete 90");
    delete(90, tree); draw(tree);
    
    puts("\n\nDelete 60");
    delete(60, tree); draw(tree);
    
    puts("\n\nDelete 40");
    delete(40, tree); draw(tree);
    
    puts("\n\nInsert 20");
    insert(20, tree); draw(tree);
    
    puts("\n\nDelete 50");
    delete(50, tree); draw(tree);
    
    puts("\n\nSearch 30");
    printf("\n%sFound: %d\n", search(30, tree) ? "" : "Not ", search(30, tree) ? search(30, tree)->value : -1);
    
    puts("\n\nSearch 99");
    printf("\n%sFound: %d\n", search(99, tree) ? "" : "Not ", search(99, tree) ? search(99, tree)->value : -1);
    
    puts("\n\nIn-order tree traversal:");
    inorder(tree); puts("");

    puts("\n\nInsert 40");
    insert(40, tree); draw(tree);
    
    puts("\n\nIn-order tree traversal:");
    inorder(tree); puts("");
    
    puts("\n\nPre-order tree traversal:");
    preorder(tree); puts("");
    
    puts("\n\nPost-order tree traversal:");
    preorder(tree); puts("");
    
    puts("\n\nTree structure:");
    draw(tree);
    
    puts("\n\nMax value:");
    printf("%s%d\n", max_node(tree) ? "" : "Tree is empty: ", max_node(tree) ? max_node(tree)->value : -1);

    puts("\n\nInsert 25");
    insert(25, tree); draw(tree);
    
    puts("\n\nMin value:");
    printf("%s%d\n", min_node(tree) ? "" : "Tree is empty: ", min_node(tree) ? min_node(tree)->value : -1);
	
	puts("End of test\n");
}

int main(int argc, char *argv[]) {
    struct tree *tree = (struct tree *)malloc(sizeof(struct tree));
    if (tree == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    test(tree); return 0;

    int choice = -1;
    while (choice) {
        puts("\n----- Binary Search Tree -------");
        puts("1. Insert value to BST");
        puts("2. Delete value from BST");
        puts("3. Search value in BST");
        puts("4. Find min value in BST");
        puts("5. Find max value in BST");
        puts("6. Inorder BST traversal");
        puts("7. Preorder BST traversal");
        puts("8. Postorder BST traversal");
        puts("9. Draw BST graphically");
        puts("0. Exit");
        
        printf("\nEnter your choice: ");
        if (scanf("%d", &choice) == 0)
            break;
        
        int value;
        switch (choice) {
        case 1:
        {
            printf("Enter value to be inserted: ");
            scanf("%d", &value);
            if (insert(value, tree) != -1)
                printf("\nInserted %d\n", value);
            break;
        }
        case 2:
        {
            printf("Enter value to be deleted: ");
            scanf("%d", &value);
            if (delete(value, tree) != -1)
                printf("\nDeleted %d\n", value);
            break;
        }
        case 3:
        {
            printf("Enter value to be searched: ");
            scanf("%d", &value);
            struct node *node;
            if (node = search(value, tree))
                printf("\nFound %d\n", node->value);
            break;
        }
        case 4:
        {
            struct node *node;
            if (node = min_node(tree))
                printf("\nMinimum node value: %d\n", node->value);
            break;
        }
        case 5:
        {
            struct node *node;
            if (node = max_node(tree))
                printf("\nMaximum node value: %d\n", node->value);
            break;
        }
        case 6:
            puts("\nIn-order tree traversal:");
            inorder(tree);
            puts("");
            break;
        case 7:
            puts("\nPre-order tree traversal:");
            preorder(tree);
            puts("");
            break;
        case 8:
            puts("\nPost-order tree traversal:");
            postorder(tree);
            puts("");
            break;
        case 9:
            puts("\nTree structure:");
            draw(tree);
            break;
        case 0:
            break;
        }
    }
    
    free(tree);
    
    return EXIT_SUCCESS;
}
