#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define hcount(x) (((x) >> 1) + ((x) & 1))

struct tree;
struct node;

/**
 * @param n size of input chain of values
 * @return int number of nodes in tree
 */
static inline int size(int n);

/**
 * @param n size of input chain of values
 * @return int tree height 
 */
static inline int height(int n);

/*
 * @return int logarithm of 10
 */
static inline int mylog10(uint32_t v);

/*
 * @return int power of 10
 */
static inline int mypow10(uint32_t v);

/*
 * @return int concatenated left and right
 */
static inline int concat(uint32_t l, uint32_t r);

static inline int hash(int val);

/*
 * @brief build Merkle tree structure
 */
static void build(struct tree *merkle, size_t n, uint32_t vals[static n]);

/*
 * @brief Get merkle tree root hash
 */
static inline int root(struct tree *merkle);

/*
 * @brief Request merkle proof
 */
int request(struct tree *merkle, struct tree *proof, int val);

/*
 * @brief Verify merkle proof for the value
 */
int validate(struct tree *proof, int root, int val);

/*
 * @brief Helper function to visualize merkle tree
 */
void draw(struct tree *merkle, int n);

