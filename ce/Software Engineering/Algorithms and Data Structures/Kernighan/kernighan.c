#include <stdio.h>
#include <stdint.h>

int kernighan(int x) {
    int count = 0;
    while (x > 0) {
        x &= x - 1;
        count++;
    }
    return count;
}

int main() {
    uint8_t var = 0x1f;
    int bits = kernighan(var);
    printf("Number of set bits in %#x: %d\n", var, bits);
    return 0;
}
