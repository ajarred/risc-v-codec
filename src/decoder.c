#include "../include/decoder.h"

void test1(instruction* i, int x) {
    i->x = x;
    printf("%d\n", x);
}