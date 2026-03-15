#include "merkle_tree.h"

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

int size(int n) {
    int s = n;
    while ((n = hcount(n)) > 1)
        s += n;
    return s + 1;
}

int height(int n) {
    int h = 0;
    while ((1 << h) <= n)
        h++;
    return h;
}

int mylog10(uint32_t val) {
    int res = 1;
    while (val /= 10)
        res++;
    return res;
}

int mypow10(uint32_t val) {
    int res = 1;
    while (val--)
        res *= 10;
    return res;
}

int concat(uint32_t l, uint32_t r) {
    return l * mypow10(mylog10(l)) + r;
}

static inline int hash(int val) {
    return val;
}

void build(struct tree *merkle, size_t n, uint32_t chain[static n]) {
    int s = size(n);
    printf("size: %d\n", s);
    merkle->arr = calloc(s, sizeof(struct node));
    if (merkle->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return;
    }
    
    merkle->height = height(n);
    merkle->count = size(n);
    
    for (int i = 0; i < n; i++)
        merkle->arr[i] = (struct node){ hash(chain[i]), 0, i };
    
    int cnt = n;
    int beg = n, end = 0;
    for (int i = 0; i < merkle->height; i++) {
        int prev_cnt = cnt;
        end = beg + (cnt = hcount(cnt));
        
        int j = 0;
        while (beg < end) {
            int next = 1;
            if (end - beg == 1)
                next = !(prev_cnt % 2);
            
            int left = merkle->arr[beg - prev_cnt + j].hash;
            int right = merkle->arr[beg + next - prev_cnt + j].hash;
            int temp = concat(left, right);
            
            merkle->arr[beg] = (struct node){ hash(temp), i + 1, j++ };
            
            beg++;
        }
    }
}

int root(struct tree *merkle) {
    return merkle->arr[merkle->count - 1].hash;
}

int request(struct tree *merkle, struct tree *proof, int val) {
    int size = merkle->height;
    proof->arr = calloc(size, sizeof(int));
    if (proof->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return -1;
    }
    
    proof->height = merkle->height;
    proof->count = merkle->height;
    
    int count = 0, pos = 0;
    for (int i = 0; i < merkle->height; i++) {
        while (merkle->arr[pos].level < i) pos++;
        int j = 0;
        while (merkle->arr[pos + j].level == i) {
            if (hash(val) == merkle->arr[j].hash) {
                if (merkle->arr[pos + j].pos % 2)
                    proof->arr[count++] = merkle->arr[pos + j - 1];
                else if (merkle->arr[pos + j + 1].level > i)
                    proof->arr[count++] = merkle->arr[pos + j];
                else
                    proof->arr[count++] = merkle->arr[pos + j + 1];
                break;
            }
        }
    }
}
int validate(struct tree *proof, int root, int val) {
    int hval = hash(val);
    for (size_t i = 0; i < proof->count; i++) {
        int left, right;
        if (proof->arr[i].pos % 2) {
            left = hval;
            right = proof->arr[i].hash;
        } else {
            left = proof->arr[i].hash;
            right = hval;
        }
        int temp = concat(left, right);
        hval = hash(temp);
    }

    return hval == root;
}

void draw(struct tree *merkle, int n) {
    int prev_cnt = n, cnt = n;
    int beg = 0, end = n;
    for (int i = 0; i < merkle->height + 1; i++) {
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
    request(merkle, proof, val);
    for (size_t i = 0; i < proof->count; i++)
        printf("%d ", proof->arr[i].hash);
    printf("\n");
    
    printf("Validation of value: ");
    int res = validate(proof, r, val);
    printf("%s\n", res ? "OK" : "ERR");
    
    free(merkle->arr);
    free(proof->arr);
}

int main() {
    uint32_t chain[] = { 1, 2, 3, 4, 5, 6, 7 };
    const size_t n = sizeof(chain) / sizeof(uint32_t);
    
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
    
    test(merkle, n, chain, proof);

    free(proof);
    free(merkle);

    return 0;
}

