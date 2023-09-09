#include "../include/decoder.h"
#include "../include/encoder.h"

int main() {
    printf("RISC-V Instruction Codec\n"
    "Please input a hex string starting with 0x\n"
    "Max 8-characters excl. 0x (e.g., 0x1989)\n"
    "Press Ctrl-C to terminate\n");
    while (1) {
        char input[255];
        unsigned int n;
        printf("risc>");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; 
        }
        if (strncmp(input, "0x", 2) == 0) {
            if (convertStrToUint(input, &n)) {
                instruction* risc = malloc(sizeof(instruction));
                risc->input = n;
                decodeInstruction(risc);
                free(risc); 
            }
        } else if (isValidInstruction(input)) {
            instruction* risc = malloc(sizeof(instruction));
            obtainInstruction(risc, input);
            // printf("instr = %s\n", risc->instr);
            // printf("type = %d\n", risc->type);
            obtainRd(risc, input);
            free(risc);
        } else {
            fprintf(stderr, "Invalid input.\n");
        }
    }
    return 0;
}