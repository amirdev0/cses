#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

#define hcount(x) (((x) >> 1) + ((x) & 1))

struct node {
    uint32_t hash;
    size_t level;
    size_t pos;
};

struct tree {
    struct node *arr;
    size_t height;
    size_t count;
};

/*
 * @brief build Merkle tree structure
 */
void build(struct tree *merkle, size_t n, uint32_t chain[static n]);

/*
 * @brief Request merkle proof
 */
int request(struct tree *merkle, struct tree *proof, int val);

/*
 * @brief Verify merkle proof for the value
 */
int validate(struct tree *proof, int root, int val);


