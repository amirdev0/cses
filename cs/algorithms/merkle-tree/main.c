#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include "bottom/bottom_merkle.h"

// #define MN_WIDTH 256

#ifdef BENCHMARK
uint64_t get_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (uint64_t)ts.tv_sec * 1000000000 + ts.tv_nsec;
}

void benchmark_build(size_t num_runs, merkle_t ** const merkle)
{
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    for (size_t i = 0; i < num_runs; i++) {
        uint32_t arr[N];
        for (size_t j = 0; j < N; j++)
            arr[j] = rand() % 10000;  

        (*merkle) = realloc((*merkle), sizeof(merkle_t) + N * sizeof(node_t));
        if (merkle == NULL) {
            fprintf(stderr, "Failed to allocate memory: %zuB\n", sizeof(merkle_t) + N * sizeof(node_t));
            exit(EXIT_FAILURE);
        }
        
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

void benchmark_generate(size_t num_runs, merkle_t ** const merkle, proof_t ** const proof)
{
    uint64_t total_time = 0;
    uint64_t start_time, end_time;
    double seconds = 0;

    uint32_t arr[N];
    for (size_t j = 0; j < N; j++)
        arr[j] = rand() % 10000;

    (*merkle) = realloc((*merkle), sizeof(merkle_t) + N * (sizeof(node_t)));
    if (merkle == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }

    (*merkle) = build((*merkle), N, arr);
    if ((*merkle) == NULL) {
        fprintf(stderr, "Failed to build tree");
        return;
    }
    
    for (size_t i = 0; i < num_runs; i++) {
        node_t target = (node_t){ arr[rand() % N] };
        (*proof) = realloc((*proof), sizeof(merkle_t) + (*merkle)->size * (sizeof(node_t) + sizeof(side_e)));
        if (proof == NULL) {
            fprintf(stderr, "Error in memory allocation\n");
            exit(EXIT_FAILURE);
        }
        
        start_time = get_time_ns();
        (*proof) = generate((*proof), (*merkle), target);
        end_time = get_time_ns();
        
        total_time += end_time - start_time;
    }
    seconds = (double)total_time / 1000000000.0;

    puts("Build Merkle proof:");
    printf("Total time for %zu runs: %lf seconds\n", num_runs, seconds);
    printf("Average time per run: %lf microseconds\n", (seconds / num_runs) * 1000000.0);
}

void benchmark(size_t cycles, merkle_t ** const merkle, proof_t ** const proof)
{
    benchmark_build(cycles, merkle);
    benchmark_generate(cycles, merkle, proof);
    puts("");
}
#endif

void test(merkle_t ** const merkle, proof_t ** const proof, size_t n, uint32_t chain[static n])
{
    printf("Chain of %zu values:\n", n);
    for (size_t i = 0; i < n; i++)
        printf(MERKLE_NODE_FMT " -> ", chain[i]);
    printf("x\n");

    (*merkle) = build((*merkle), n, chain);
    
    puts("Merkle tree:");
    // draw((*merkle), n);
    puts("\n");
    
    node_t root = (*merkle)->root;
    printf("Merkle tree root hash: " MERKLE_NODE_FMT "\n", root.hash);
    
    node_t target = { chain[n - 3] };
    printf("Generated proof for value " MERKLE_NODE_FMT ": ", target.hash);
    (*proof) = generate((*proof), (*merkle), target);
    for (size_t i = 0; i < (*proof)->size; i++)
        printf(MERKLE_NODE_FMT " ", (*proof)->entry[i].node.hash);
    printf("\n");
    
    printf("Validation of target value: ");
    node_t node = validate(target, (*proof));
    printf("%s\n", node.hash == (*merkle)->root.hash ? "OK" : "ERR");
}

int main(void)
{
    merkle_t *merkle = malloc(sizeof(proof_t));
    if (merkle == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        exit(EXIT_FAILURE);
    }
    
    proof_t *proof = malloc(sizeof(proof_t));
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
#endif

    uint32_t chain[] = { 1, 2, 3, 4, 5, 6, 7 };
    size_t n = sizeof(chain) / sizeof(*chain);
    test(&merkle, &proof, n, chain);

    free(proof);
    free(merkle);

    return EXIT_SUCCESS;
}

