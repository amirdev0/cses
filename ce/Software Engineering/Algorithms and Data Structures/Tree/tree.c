#include <stdio.h>
#include <stdlib.h>

#define hcount(x) (((x) >> 1) + ((x) & 1))

struct node {
    int hash;
    int level;
    int pos;
};

struct tree {
    struct node *arr;
    int height;
    int count;
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

int mylog10(int val) {
    int res = 1;
    while (val /= 10)
        res++;
    return res;
}

int mypow10(int val) {
    int res = 1;
    while (val--)
        res *= 10;
    return res;
}

int concat(int val1, int val2) {
    return val1 * mypow10(mylog10(val1)) + val2;
}

int hash(int val) {
    return val;
}

void build(struct tree *merkle, int n, int vals[n]) {
    int s = size(n);
    merkle->arr = calloc(s, sizeof(struct node));
    if (merkle->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return;
    }
    
    merkle->height = height(n);
    merkle->count = size(n);
    
    for (int i = 0; i < n; i++)
        merkle->arr[i] = (struct node){ hash(vals[i]), 0, i };
    
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
            
            merkle->arr[beg].hash = hash(temp);
            merkle->arr[beg].level = i + 1;
            merkle->arr[beg].pos = j++;
            
            beg++;
        }
    }
}

int obtain(struct tree *merkle) {
    return merkle->arr[merkle->count - 1].hash;
}

void request(struct tree *merkle, struct tree *proof, int val) {
    int size = merkle->height;
    proof->arr = calloc(size, sizeof(int));
    if (proof->arr == NULL) {
        fprintf(stderr, "Error in memory allocation\n");
        return;
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
    for (int i = 0; i < proof->count; i++) {
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

void test(struct tree *merkle, int n, int vals[n], struct tree *proof) {
    printf("Chain of %3d values: ", n);
    for (int i = 0; i < n; i++)
        printf("%6d ", vals[i]);
    printf("\n");
    
    build(merkle, n, vals);
    
    printf("Corresponding hashes: ");
    for (int i = 0; i < n; i++)
        printf("%6d ", merkle->arr[i].hash);
    printf("\n\n");
    
    puts("Merkle tree:");
    draw(merkle, n);
    printf("\n\n");
    
    int root = obtain(merkle);
    printf("Obtained root hash: %d\n", root);
    
    int val = vals[0];
    printf("Requested proof for value %d: ", val);
    request(merkle, proof, val);
    for (int i = 0; i < proof->count; i++)
        printf("%d ", proof->arr[i].hash);
    printf("\n");
    
    printf("Validation of value: ");
    int res = validate(proof, root, val);
    printf("%s\n", res ? "OK" : "ERR");
    
    free(merkle->arr);
    free(proof->arr);
}

int main() {
    int vals[] = { 1, 2, 3, 4, 5, 6, 7 };
    const int n = sizeof(vals) / sizeof(int);
    
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
    
    printf("%d", mmm(10));
    return 0;
        
    test(merkle, n, vals, proof);

    free(proof);
    free(merkle);
    return 0;
}

