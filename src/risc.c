#include "../include/decoder.h"
#include "../include/encoder.h"

int main() {
    while (1) {
        char input[255];
        unsigned int n;
        printf("risc>");
        scanf("%s", input);
        if (convertStrToUint(input, &n)) {
            instruction* risc = malloc(sizeof(instruction));
            risc->input = n;
            parseOpcode(risc); 
            parseFunct3(risc);
            parseFunct7(risc);
            parseRd(risc);
            parseRs1(risc);
            parseRs2(risc);
            printInstructions(risc);
            free(risc); 
        }
    }
    return 0;
}