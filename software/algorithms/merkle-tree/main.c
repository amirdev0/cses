#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "merkle_tree.h"

void test(struct tree ** const merkle, size_t n, uint32_t chain[static n], struct tree ** const proof)
{
    printf("Chain of %zu values:\t", n);
    for (size_t i = 0; i < n; i++)
        printf("%" PRIu32 " -> ", chain[i]);
    printf("x\n");

    (*merkle) = build((*merkle), n, chain);
    
    printf("Corresponding hashes:\t");
    for (size_t i = 0; i < n; i++)
        printf("%" PRIu32 " -> ", (*merkle)->arr[i].hash);
    printf("x\n\n");
    
    puts("Merkle tree:");
    draw((*merkle), n);
    puts("\n");
    
    uint32_t r = root(*merkle);
    printf("Merkle tree root hash: %" PRIu32 "\n", r);
    
    uint32_t val = chain[n - 3];
    printf("Requested proof for value %" PRIu32 ": ", val);
    (*proof) = request((*proof), (*merkle), val);
    for (size_t i = 0; i < (*proof)->count; i++)
        printf("%d ", (*proof)->arr[i].hash);
    printf("\n");
    
    printf("Validation of value: ");
    int res = validate((*proof), r, val);
    printf("%s\n", res ? "OK" : "ERR");
}

uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void benchmark_build(size_t num_runs, struct tree ** const merkle)
{
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    for (size_t i = 0; i < num_runs; i++) {
        uint32_t arr[N];
        for (size_t j = 0; j < N; j++)
            arr[j] = rand() % 10000;  

        struct tree *new_merkle = realloc((*merkle), sizeof(struct tree) + size(N) * sizeof(struct node));
        if (new_merkle == NULL) {
            fprintf(stderr, "Error in memory allocation\n");
            exit(EXIT_FAILURE);
        }
        (*merkle) = new_merkle;
        
        start_time = get_time_ns();
        (*merkle) = build((*merkle), N, arr);
        end_time = get_time_ns();

        total_time += end_time - start_time;        
    }
    seconds = (double)total_time / 1000000000.0;

    puts("Build Merkle tree:");
    printf("Total time for %zu runs: %lf seconds\n", num_runs, seconds);
    printf("Average time per run: %lf microseconds\n", (seconds / num_runs) * 1000000.0);
}

void benchmark_request(size_t num_runs, struct tree ** const merkle, struct tree ** const proof)
{
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    uint32_t arr[N];
    for (size_t j = 0; j < N; j++)
        arr[j] = rand() % 10000;

    struct tree *new_merkle = realloc((*merkle), sizeof(struct tree) + size(N) * sizeof(struct node));
    if (new_merkle == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    (*merkle) = new_merkle;
    (*merkle) = build((*merkle), N, arr);

    for (size_t i = 0; i < num_runs; i++) {
        int val = arr[rand() % N];
        struct tree *new_proof = realloc((*proof), sizeof(struct tree) + (*merkle)->height * sizeof(struct node));
        if (new_proof == NULL) {
            fprintf(stderr, "Error in memory allocation\n");
            exit(EXIT_FAILURE);
        }
        (*proof) = new_proof;
        
        start_time = get_time_ns();
        (*proof) = request((*proof), (*merkle), val);
        end_time = get_time_ns();
        
        total_time += end_time - start_time;
    }
    seconds = (double)total_time / 1000000000.0;

    puts("Build Merkle proof:");
    printf("Total time for %zu runs: %lf seconds\n", num_runs, seconds);
    printf("Average time per run: %lf microseconds\n", (seconds / num_runs) * 1000000.0);
}

void benchmark(size_t cycles, struct tree ** const merkle, struct tree ** const proof)
{
    benchmark_build(cycles, merkle);
    benchmark_request(cycles, merkle, proof);  
}

int main(void)
{
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
    
#ifdef BENCHMARK
    puts("Enter cycle number to launch benchmark (max 100,000):");
    size_t cycles = 0;
    if (scanf("%zu", &cycles)) {
        cycles = cycles > (size_t)100000 ? (size_t)100000 : cycles;
        printf("\nBenchmark input size: %zu\n\n", N);
        benchmark(cycles, &merkle, &proof);;
    }
    
#else

    uint32_t chain[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t n = sizeof(chain) / sizeof(*chain);
    test(&merkle, n, chain, &proof);
#endif

    free(proof);
    free(merkle);

    return EXIT_SUCCESS;
}

