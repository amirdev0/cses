#include "bottom_merkle.h"
#include <assert.h>
/**
 * @param n size of input chain of values
 * @return size_t number of nodes in tree
 */
size_t size(size_t n)
{
    size_t s = n;
    while ((n = hcount(n)) > 1)
        s += n;
    return s + 1;
}

/**
 * @param n size of input chain of values
 * @return size_t tree height
 */
static inline size_t llog2(size_t n)
{
    size_t h = 0;
    while (n >>= 1) h++;
    return h;
}

/**
 * @param n size of input chain
 * @brief number of nodes in tree
 */
size_t size(size_t n) {
    size_t lss = 2 * (1 << llog2(n)) - 1;
    size_t rss = 2 * (1 << llog2(n - (1 << llog2(n)))) - 1;
    return lss + rss + 1;
}

/**
 * @brief concatenated left and right
 */
static inline struct node concat(struct node left, struct node right) {
    return (struct node){ left.hash + right.hash, 0 };
}

/**
 * @brief simple stub hash
 */
static inline struct node hash(struct node node) {
    return node;
}

struct tree* build(struct tree *merkle, size_t n, const uint32_t chain[static n])
{
#ifndef BENCHMARK
    merkle = realloc(merkle, sizeof(struct tree) + size(n) * sizeof(struct node));
    if (merkle == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    }
#endif

    size_t lst_off = 1 << llog2(n);
    for (size_t i = 0; i < lst_off; i++)
        merkle->node[i] = (struct node){ chain[i], 0 };
    for (size_t i = 0; i <= llog2(n); i++)
        merkle->node[lst_off + i] = hash(concat(merkle->node[2 * i + i], merkle->node[2 * i + i + 1]));

    size_t rst_off = size(lst_off);
    for (size_t i = 0; lst_off + i < n; i++)
        merkle->node[rst_off + i] = (struct node){ chain[lst_off + i], 0 };
    for (size_t i = 0; i < llog2(n - (1 << llog2(n))); i++)
        merkle->node[lst_off + rst_off + i] = hash(concat(merkle->node[2 * i + i], merkle->node[2 * i + i + 1]));

    rst_off = size(rst_off);
    merkle->root = hash(concat(merkle->node[lst_off - 1], merkle->node[lst_off + rst_off]));
    merkle->count = size(n);
    return merkle;
}

struct tree* request(struct tree *proof, const struct tree* merkle, struct node target)
{
#ifndef BENCHMARK
    proof = realloc(proof, sizeof(struct tree) + (llog2(merkle->count) + 1) * sizeof(struct node));
    if (proof == NULL) {
        perror("realloc");
        exit(EXIT_FAILURE);
    };
#endif
    
    size_t idx = 0;
    const struct node hval = hash(target);
    size_t count = hcount(merkle->count);
    while (idx < count && merkle->node[idx].hash != hval.hash)
            idx++;

    if (idx == count - 1)
        return 0;
    
    proof->count = 1 << (llog2(merkle->count) + 1);
    return proof;
}

int32_t validate(const struct tree *proof, struct node value)
{
    assert(proof != NULL);
    
    struct node target = hash(value);
    for (size_t i = 0; i < proof->count; i++) {
        struct node temp;
        if (proof->node[i].idx % 2)
            target = concat(target, proof->node[i]);
        else
            target = concat(proof->node[i], target);
        target = hash(target);
    }

    return proof->root.hash == target.hash;
}

// void draw(const struct tree *merkle, size_t n)
// {
//     size_t prev_cnt = n, cnt = n;
//     size_t beg = 0, end = n;
//     for (size_t i = 0; i < merkle->height + 1; i++) {
//         printf("%*s", 4 << i, "");

//         while (beg < end) {
//             printf("%4d", merkle->arr[beg++].hash);
//             int k = (1 << (i + 1)) - 1;
            
//             if (end - beg == 1 && prev_cnt % 2 == 1)
//                 k -= (1 << i) - 1;
//             printf("%*s", 4 * k, "");
//         }
        
//         prev_cnt = cnt;
//         end += (cnt = hcount(cnt));
//         printf("\n");
//     }
// }

