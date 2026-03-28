#include "bottom_merkle.h"

static inline size_t llog2(size_t n)
{
    size_t h = 0;
    while (n >>= 1)
        h++;
    return h;
}

/**
 * @brief concatenated left and right
 */
static inline node_t concat(node_t left, node_t right) {
    return (node_t){ left.hash + right.hash };
}

/**
 * @brief simple stub hash
 */
static inline node_t hash(node_t node) {
    return node;
}

merkle_t* build(merkle_t *merkle, size_t n, const uint32_t chain[static n])
{
#ifndef BENCHMARK
    merkle = realloc(merkle, sizeof(merkle_t) + n * sizeof(node_t));
    if (merkle == NULL) {
        perror("realloc");
        return NULL;
    }
#endif

    // Write chain hashes to merkle nodes
    for (size_t i = 0; i < n; i++)
        merkle->node[i] = hash((node_t){chain[i]});
    merkle->size = n;

    node_t *buffer = malloc(merkle->size * sizeof(node_t));
    if (buffer == NULL) {
        perror("malloc");
        return NULL;
    }

    memcpy(buffer, merkle->node, merkle->size * sizeof(node_t));

    while (n > 1) {
        // Calculate next level hashes
        for (size_t i = 0; i < n / 2; i++)
            buffer[i] = hash(concat(buffer[2 * i], buffer[2 * i + 1]));
        if (n % 2 == 1)
            buffer[n / 2] = hash(concat(buffer[n - 1], buffer[n - 1]));
        n = n / 2 + n % 2;
    }

    merkle->root = buffer[0];

    free(buffer);

    return merkle;
}

proof_t* generate(proof_t *proof, const merkle_t *merkle, node_t target)
{
#ifndef BENCHMARK
    proof = realloc(
        proof,
        sizeof(proof_t) + (llog2(merkle->size) + 1) * (sizeof(node_t) + sizeof(side_e))
    );
    if (proof == NULL) {
        perror("realloc");
        return NULL;
    };
#endif

    // Search target node index
    size_t index = merkle->size + 1;
    for (size_t i = 0; i < merkle->size; i++)
        if (merkle->node[i].hash == target.hash)
            index = i;
    if (index > merkle->size)
        return NULL;

    node_t *buffer = malloc(merkle->size * sizeof(node_t));
    if (buffer == NULL) {
        perror("malloc");
        return NULL;
    }

    memcpy(buffer, merkle->node, merkle->size * sizeof(node_t));

    proof->size = llog2(merkle->size) + 1;
    size_t n = merkle->size;
    for (size_t i = 0; i < proof->size; i++) {
        if (index != n - 1) {
            proof->entry[i].node = buffer[index - 2 * (index % 2) + 1];
            proof->entry[i].side = index % 2 == 0 ? RIGHT : LEFT;
        } else {
            proof->entry[i].node = buffer[index - (n % 2 == 0)];
            proof->entry[i].side = n % 2 == 0 ? LEFT : NONE;
        }

        // Calculate next level hashes
        for (size_t j = 0; j < n / 2; j++)
            buffer[j] = hash(concat(buffer[2 * j], buffer[2 * j + 1]));
        if (n % 2 == 1)
            buffer[n / 2] = hash(concat(buffer[n - 1], buffer[n - 1]));
        n = n / 2 + n % 2;
        index = index / 2;
}

    free(buffer);
    
    return proof;
}

node_t validate(node_t target, const proof_t *proof)
{
    for (size_t i = 0; i < proof->size; i++) {
        switch (proof->entry[i].side) {
            case NONE:
                target = hash(concat(target, target));
                break;
            case LEFT:
                target = hash(concat(proof->entry[i].node, target));
                break;
            case RIGHT:
                target = hash(concat(target, proof->entry[i].node));
                break;
            default:
                return (node_t){ 0 };
        }
    }
    return target;
}

