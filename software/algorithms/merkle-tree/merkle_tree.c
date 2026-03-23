#include "merkle_tree.h"

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
size_t height(size_t n)
{
    size_t h = 0;
    while (((size_t)1 << h) <= n)
        h++;
    return h;
}

/**
 * @return uint32_t concatenated left and right
 */
static inline uint32_t concat(uint32_t l, uint32_t r) {
    return l + r;
}

/**
 * @return uint32_t simple stub hash
 */
static inline uint32_t hash(uint32_t val) {
    return val;
}

uint32_t root(const struct tree *merkle) {
    return merkle->arr[merkle->count - 1].hash;
}

int32_t build(struct tree *merkle, size_t n, const uint32_t chain[static n])
{
#ifndef BENCHMARK
    merkle->arr = calloc(size(n), sizeof(struct node));
    if (merkle->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return -1;
    }
#endif

    merkle->height = height(n);
    merkle->count = size(n);
    
    for (size_t i = 0; i < n; i++)
        merkle->arr[i] = (struct node){ hash(chain[i]), 0, i };
    
    size_t cnt = n;
    size_t beg = n, end = 0;
    for (size_t i = 0; i < merkle->height; i++) {
        size_t prev_cnt = cnt;
        end = beg + (cnt = hcount(cnt));
        
        size_t j = 0;
        while (beg < end) {
            int32_t next = 1;
            if (end - beg == 1)
                next = !(prev_cnt % 2);
            
            uint32_t left = merkle->arr[beg - prev_cnt + j].hash;
            uint32_t right = merkle->arr[beg + next - prev_cnt + j].hash;
            uint32_t temp = concat(left, right);
            
            merkle->arr[beg++] = (struct node){ hash(temp), i + 1, j++ };
        }
    }

    return 0;
}

int32_t request(struct tree *proof, const struct tree *merkle, uint32_t val)
{
#ifndef BENCHMARK
    proof->arr = calloc(merkle->height, sizeof(struct node));
    if (proof->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return -1;
    }
#endif

    size_t idx = 0;
    const uint32_t hval = hash(val);
    size_t count = hcount(merkle->count);
    while (idx < count && merkle->arr[idx].hash != hval)
            idx++;

    if (idx == count - 1)
        return 0;
    
    size_t j = idx;
    size_t nidx = count;
    for (size_t i = 0; i < merkle->height; i++) {
        uint8_t is_odd_pos = merkle->arr[idx].pos % 2;
        uint8_t is_next_level = merkle->arr[idx + 1].level <= i;
        proof->arr[i] = merkle->arr[idx - is_odd_pos + is_next_level];
        idx = nidx + (j /= 2);
        nidx += (count = hcount(count));
    }
    
    return proof->count = merkle->height;
}

int32_t validate(const struct tree *proof, uint32_t root, uint32_t val)
{
    uint32_t hval = hash(val);
    for (size_t i = 0; i < proof->count; i++) {
        uint32_t left, right;
        if (proof->arr[i].pos % 2) {
            left = hval;
            right = proof->arr[i].hash;
        } else {
            left = proof->arr[i].hash;
            right = hval;
        }
        uint32_t temp = concat(left, right);
        hval = hash(temp);
    }

    return hval == root;
}

void draw(struct tree *merkle, size_t n)
{
    size_t prev_cnt = n, cnt = n;
    size_t beg = 0, end = n;
    for (size_t i = 0; i < merkle->height + 1; i++) {
        printf("%*s", 4 << i, "");

        while (beg < end) {
            printf("%4d", merkle->arr[beg++].hash);
            int k = (1 << (i + 1)) - 1;
            
            if (end - beg == 1 && prev_cnt % 2 == 1)
                k -= (1 << i) - 1;
            printf("%*s", 4 * k, "");
        }
        
        prev_cnt = cnt;
        end += (cnt = hcount(cnt));
        printf("\n");
    }
}

