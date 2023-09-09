#include "../include/decoder.h"
#include "../include/encoder.h"

#ifdef _WIN32

static char buffer[2048];

char* readline(char* prompt) {
  fputs(prompt, stdout);
  fgets(buffer, 2048, stdin);
  char* cpy = malloc(strlen(buffer)+1);
  strcpy(cpy, buffer);
  cpy[strlen(cpy)-1] = '\0';
  return cpy;
}

void add_history(char* unused) {}

#else
#include <editline/readline.h>
#include <editline/history.h>
#endif

int main() {
    printf("RISC-V Instruction Codec v.0.0.1\n"
    "Input options:\n"
    "Hex Number (e.g., 0x3100B3)\n"
    "OR\n"
    "Assembly Instruction (e.g., add x1, x2, x3)\n"
    "Press Ctrl-C to terminate\n");
    while (1) {
        char* input = readline("risc> ");
        add_history(input);

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
        free(input);
    }
    return 0;
}