#include "../include/decoder.h"
#include "../include/encoder.h"
#include "../include/unit-test.h"

#ifdef _WIN32

void clearScreen() {
    system("cls");
}

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

#elif __APPLE__
#include <editline/readline.h>
void clearScreen() {
    system("clear");
}

#else 
#include <editline/history.h>
void clearScreen() {
    system("clear");
}

#endif

void listAvailableInstructions();
void listAvailableCommands();

int main() {
    printf("RISC-V Instruction Codec v.0.0.1\n"
        "Input hex or assembly instructions\n"
        "Input help for more information\n");
    while (1) {
        char* input = readline("risc> ");
        add_history(input);

        if (strncmp(input, "0x", 2) == 0) {
            decodeInstruction(input);
        } else if (isValidInstruction(input)) {
            encodeInstruction(input);
        } else if (strcmp(input, "clear") == 0) {
            clearScreen();
        } else if (strcmp(input, "list") == 0) {
            listAvailableInstructions();
        } else if (strcmp(input, "help") == 0) {
            listAvailableCommands();
        } else if (strcmp(input, "quit") == 0 ||
                   strcmp(input, "exit") == 0) {
            free(input);
            break;
        } else if(strcmp(input, "test") == 0) {
            runUnitTest();
        } else {
            fprintf(stderr, "Invalid input.\n");
        }
        free(input);
    }
    return 0;
}

void listAvailableInstructions() {
    printf("Available instruction formats:\n"
    "R-Type Instructions: instr rd, rs1, rs2\n"
    "- add, sub, xor, and, sll, srl, sra, slt, sltu\n"
    "I-Type Immediate Instructions: instr rd, rs1, imm\n"
    "- addi\n"
    "I-Type Load Instructions: instr rd, imm(rs1)\n"
    "- ld\n"
    "S-Type Instructions: instr rs2, imm(rs1)\n"
    "- sd\n"
    "B-Type Instructions: instr rs1, rs2, imm\n"
    "- beq\n"
    "J-Type Instructions: instr rd, imm\n"
    "- jal\n"
    "Jump Immediate Instructions: instr rd, imm\n"
    "- jalr\n"
    "U-Type Instructions: instr rd, imm\n"
    "- lui, auipc\n"
    "Environment Instructions: instr rd, imm\n"
    "- ecall, ebreak\n"
    );
}          

void listAvailableCommands() {
    printf("Available commands:\n"
    "hex number (e.g., 0x3100B3) = converts to assembly instruction\n"
    "assembly instruction (e.g., add x1, x2, x3) = converts to hex number\n"
    "clear = clears the screen\n"
    "list = lists all available assembly instruction formats\n"
    "quit = exits the program\n");
}