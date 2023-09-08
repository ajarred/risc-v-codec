#include "../include/decoder.h"
#include "../include/encoder.h"

int main() {
    printf("RISC-V Instruction Decoder\n"
    "Please input a hex string starting with 0x\n"
    "Max 8-characters excl. 0x (e.g., 0x1989)\n"
    "Press Ctrl-C to terminate\n");
    while (1) {
        char input[255];
        unsigned int n;
        printf("risc>");
        scanf("%s", input);
        if (convertStrToUint(input, &n)) {
            instruction* risc = malloc(sizeof(instruction));
            risc->input = n;
            decodeInstruction(risc);
            free(risc); 
        }
    }
    return 0;
}