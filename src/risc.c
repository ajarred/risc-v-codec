#include "../include/decoder.h"
#include "../include/encoder.h"

int main() {
    instruction* risc = malloc(sizeof *risc);
    int x = 4;
    int y = 3;
    test1(risc, x);
    test2(risc, y);
    return 0;
}