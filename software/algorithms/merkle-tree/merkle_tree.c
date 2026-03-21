#include <time.h>
#include "merkle_tree.h"

#define N ((size_t)10000)

/**
 * @param n size of input chain of values
 * @return size_t number of nodes in tree
 */
static inline size_t size(size_t n) {
    size_t s = n;
    while ((n = hcount(n)) > 1)
        s += n;
    return s + 1;
}

/**
 * @param n size of input chain of values
 * @return size_t tree height
 */
static inline size_t height(size_t n) {
    size_t h = 0;
    while (((size_t)1 << h) <= n)
        h++;
    return h;
}

/**
 * @return uint32_t logarithm of ten
 */
static inline uint32_t mylog10(uint32_t val) {
    uint32_t res = 1;
    while (val /= 10)
        res++;
    return res;
}

/**
 * @return uint32_t power of ten
 */
static inline uint32_t mypow10(uint32_t val) {
    uint32_t res = 1;
    while (val--)
        res *= 10;
    return res;
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

/**
 * @brief Get merkle tree root hash
 */
static inline uint32_t root(const struct tree *merkle) {
    return merkle->arr[merkle->count - 1].hash;
}

/**
 * @brief Helper function to visualize merkle tree
 */
static void draw(struct tree *merkle, size_t n)
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

void build(struct tree *merkle, const size_t n, const uint32_t chain[static n])
{
    size_t s = size(n);
    merkle->arr = calloc(s, sizeof(struct node));
    if (merkle->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return;
    }
    
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
            
            merkle->arr[beg] = (struct node){ hash(temp), i + 1, j++ };
            
            beg++;
        }
    }
}

int request(struct tree *proof, const struct tree *merkle, const uint32_t val)
{
    proof->arr = calloc(merkle->height, sizeof(struct node));
    if (proof->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return -1;
    }

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

int validate(const struct tree *proof, const uint32_t root, const uint32_t val) {
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

void test(struct tree *merkle, size_t n, uint32_t chain[static n], struct tree *proof) {
    printf("Chain of %zu values:\t", n);
    for (size_t i = 0; i < n; i++)
        printf("%" PRIu32 " -> ", chain[i]);
    printf("x\n");
    
    build(merkle, n, chain);
    
    printf("Corresponding hashes:\t");
    for (size_t i = 0; i < n; i++)
        printf("%" PRIu32 " -> ", merkle->arr[i].hash);
    printf("x\n\n");
    
    puts("Merkle tree:");
    draw(merkle, n);
    puts("\n");
    
    uint32_t r = root(merkle);
    printf("Merkle tree root hash: %" PRIu32 "\n", r);
    
    uint32_t val = chain[n - 3];
    printf("Requested proof for value %" PRIu32 ": ", val);
    request(proof, merkle, val);
    for (size_t i = 0; i < proof->count; i++)
        printf("%d ", proof->arr[i].hash);
    printf("\n");
    
    printf("Validation of value: ");
    int res = validate(proof, r, val);
    printf("%s\n", res ? "OK" : "ERR");
    
    free(merkle->arr);
    free(proof->arr);
}

uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void benchmark_build(size_t num_runs, struct tree *merkle) {
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    for (size_t i = 0; i < num_runs; i++) {
        const size_t n = 1000;
        uint32_t arr[1000];
        for (size_t j = 0; j < n; j++)
            arr[j] = rand() % 10000;
        start_time = get_time_ns();
        build(merkle, n, arr);
        end_time = get_time_ns();
        free(merkle->arr);
        total_time += end_time - start_time;
    }
    seconds = (double)total_time / 1000000000.0;

    puts("Build Merkle tree:");
    printf("Total time for %zu runs: %lf seconds\n", num_runs, seconds);
    printf("Average time per run: %lf microseconds\n", (seconds / num_runs) * 1000000.0);
}

void benchmark_proof(size_t num_runs, struct tree *merkle, struct tree *proof, size_t n, uint32_t arr[static n]) {
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    for (size_t i = 0; i < num_runs; i++) {
        int val = arr[rand() % n];
        start_time = get_time_ns();
        (void)request(proof, merkle, val);
        end_time = get_time_ns();
        free(proof->arr);
        total_time += end_time - start_time;
    }
    seconds = (double)total_time / 1000000000.0;

    puts("Build Merkle proof:");
    printf("Total time for %zu runs: %lf seconds\n", num_runs, seconds);
    printf("Average time per run: %lf microseconds\n", (seconds / num_runs) * 1000000.0);
}

void benchmark(size_t cycles, struct tree *merkle, struct tree *proof)
{
    puts("");
    printf("Benchmark input size: %zu\n\n", N);
    benchmark_build(cycles, merkle);
    puts("");
    uint32_t arr[N];
    for (size_t j = 0; j < N; j++)
        arr[j] = rand() % 10000;
    build(merkle, N, arr);
    benchmark_proof(cycles, merkle, proof, N, arr);  
}

int main() {
    uint32_t chain[] = { 1, 2, 3, 4, 5, 6, 7 };
    const size_t n = sizeof(chain) / sizeof(*chain);
    
    struct tree *merkle = malloc(sizeof(struct tree));
    if (merkle == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    
    struct tree *proof = malloc(sizeof(struct tree));
    if (proof == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    puts("Enter cycle number to launch benchmark (max 100,000):");
    size_t cycles = 0;
    if (scanf("%zu", &cycles)) {
        if (cycles > (size_t)100000)
            cycles = (size_t)100000;
        benchmark(cycles, merkle, proof);
        return 0;
    }
    
    test(merkle, n, chain, proof);

    free(proof);
    free(merkle);

    return 0;
}

