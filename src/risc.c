#include "../include/decoder.h"
#include "../include/encoder.h"

int main() {
    printf("RISC-V Instruction Codec\n"
    "Input options:\n"
    "Hex Number (e.g., 0x1989),\n"
    "Assembly Instruction (e.g., add x1, x2, x3)\n"
    "Press Ctrl-C to terminate\n");
    while (1) {
        char input[255];
        printf("risc>");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }
        if (strncmp(input, "0x", 2) == 0) {
            unsigned int n;
            if (convertStrToUint(input, &n)) {
                instruction* risc = malloc(sizeof(instruction));
                risc->input = n;
                decodeInstruction(risc);
                free(risc); 
            }
        } else if (isValidInstruction(input)) {
            instruction* risc = malloc(sizeof(instruction));
            if (obtainInstruction(risc, input)) {
                encodeInstructions(risc, input);
            }
            free(risc);
        } else {
            fprintf(stderr, "Invalid input.\n");
        }
    }
    return 0;
}