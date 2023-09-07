#include "decoder.h"
#include "encoder.h"
#include <stdlib.h>

int main() {
    instruction* risc = malloc(sizeof *risc);
    int x = 1;
    int y = 2;
    test1(risc, x);
    test2(risc, y);
    return 0;
}