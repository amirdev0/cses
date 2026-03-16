
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

#define max(a, b) ((a) >= ((b) ? ((a) : (b))))

struct node* search(int value, struct tree *tree);

int height(struct tree *tree);

struct node* find_parent_node(int value, struct tree *tree);

struct node* find_predecessor(int value, struct tree *tree);

struct node* find_successor(int value, struct tree *tree);

struct node* min_node(struct tree *tree);

struct node* max_node(struct tree *tree);

int insert(int value, struct tree *tree);

int delete(int value, struct tree *tree);

void inorder(struct tree *tree);

void preorder(struct tree *tree);

void postorder(struct tree *tree);

void draw(struct tree *tree);

