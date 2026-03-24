#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>

#define N ((size_t)10000)

#define hcount(x) (((x) >> 1) + ((x) & 1))

struct node {
    uint32_t hash;
    size_t level;
    size_t pos;
};

struct tree {
    size_t height;
    size_t count;
    struct node arr[0];
};

size_t size(size_t n);

/**
 * @brief Get merkle root
 */
uint32_t root(const struct tree *merkle);

/**
 * @brief Build merkle tree
 */
struct tree* build(struct tree *merkle, size_t n, const uint32_t chain[static n]);

/**
 * @brief Request merkle proof
 */
struct tree* request(struct tree *proof, const struct tree *merkle, uint32_t val);

/**
 * @brief Verify merkle proof for the value
 */
int32_t validate(const struct tree *proof, uint32_t root, uint32_t val);

/**
 * @brief Visualize merkle tree structure
 */
void draw(const struct tree *merkle, size_t n);

#endif //MERKLE_TREE_H
