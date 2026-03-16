Binary Search Tree
==================

Data types;

//Actual tree contains only root node
//(could be complemented by other fields)
struct tree;

//Actual node with left and right children
struct node;

//Interfaces to manipulate tree
int insert(int value, struct tree *tree);
int delete(int value, struct tree *tree);
struct node*search(int value, struct tree *tree);

struct node* max_node(struct tree *tree);
struct node* min_node(struct tree *tree);

void inorder(struct tree *tree);
void preorder(struct tree *tree);
void postorder(struct tree *tree);

struct node* find_parent_node(int value, struct tree *tree);
struct node* find_predecessor(int value, struct tree *tree);
struct node* find_successor(int value, struct tree *tree);

int height(struct tree *tree);
void draw(struct tree *tree);
void test(struct tree *tree);

