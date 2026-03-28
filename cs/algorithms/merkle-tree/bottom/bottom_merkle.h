#ifndef MERKLE_TREE_H
#define MERKLE_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbit.h>
#include <stdint.h>
#include <string.h>

#define N ((size_t)10000)

#define MERKLE_NODE_T(byte_width) struct {                               \
                                      unsigned _BitInt(byte_width) hash; \
                                  }

#ifndef MERKLE_NODE_WIDTH
#define MERKLE_NODE_WIDTH 128
#endif

#define STR_HELPER(X) #X
#define STR(X) STR_HELPER(X)
#define MERKLE_NODE_WIDTH_STR STR(MERKLE_NODE_WIDTH)

#define MERKLE_NODE_FMT "%w" MERKLE_NODE_WIDTH_STR "u"

typedef MERKLE_NODE_T(MERKLE_NODE_WIDTH) node_t;

typedef enum: uint8_t {
  NONE,
  LEFT,
  RIGHT
} side_e;

typedef struct {
  size_t size;
  struct {
      side_e side;
      node_t node;
  } entry[0];
} proof_t;

typedef struct {
    size_t size;
    node_t root;
    node_t node[0];
} merkle_t;


/**
 * @brief Build merkle tree
 */
merkle_t* build(merkle_t *merkle, size_t n, const uint32_t chain[static n]);

/**
 * @brief Generate merkle proof
 */
proof_t* generate(proof_t *proof, const merkle_t *merkle, node_t target);

/**
 * @brief Validate merkle proof for the value
 */
node_t validate(node_t target, const proof_t *proof);

#endif //MERKLE_TREE_H
