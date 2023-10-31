#include "../include/decoder.h"
#include "../include/encoder.h"
#include <assert.h>

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
void runUnitTest();

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
    "add rd, rs1, rs2\n"
    "sub rd, rs1, rs2\n"
    "addi rd, rs1, imm\n"
    "ld rd, imm(rs1)\n"
    "sd rs2, imm(rs1)\n");
}          

void listAvailableCommands() {
    printf("Available commands:\n"
    "hex number (e.g., 0x3100B3) = converts to assembly instruction\n"
    "assembly instruction (e.g., add x1, x2, x3) = converts to hex number\n"
    "clear = clears the screen\n"
    "list = lists all available assembly instruction formats\n"
    "quit = exits the program\n");
}

void runUnitTest() {
    printf("Running tests...\n\n");

    // add x0, x1, x2
    instruction* test = createDecodedInstruction(0x00208033);
    assert(test == NULL);
    free(test);

    // addi x0, x1, 1
    test = createDecodedInstruction(0x00108013);
    assert(test == NULL);
    free(test);

    test = createDecodedInstruction(0x3100B3);
    assert(strcmp(test->assemblyStr, "add x1, x2, x3") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("add x1, x2, x3");
    assert(test->input == 0x3100B3);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sub x29, x31, x30");
    assert(test->input == 0x41ef8eb3);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x41ef8eb3);
    assert(strcmp(test->assemblyStr, "sub x29, x31, x30") == 0);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x800a0513);
    assert(strcmp(test->assemblyStr, "addi x10, x20, -2048") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("addi x10, x20, -2048");
    assert(test->input == 0x800a0513);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x7ffa0513);
    assert(strcmp(test->assemblyStr, "addi x10, x20, 2047") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("addi x10, x20, 2047");
    assert(test->input == 0x7ffa0513);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x80013083);
    assert(strcmp(test->assemblyStr, "ld x1, -2048(x2)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("ld x1, -2048(x2)");
    assert(test->input == 0x80013083);
    free(test->assemblyStr);
    free(test);   

    test = createDecodedInstruction(0x7ffa3983);
    assert(strcmp(test->assemblyStr, "ld x19, 2047(x20)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("ld x19, 2047(x20)");
    assert(test->input == 0x7ffa3983);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x801fb0a3);
    assert(strcmp(test->assemblyStr, "sd x1, -2047(x31)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sd x1, -2047(x31)");
    assert(test->input == 0x801fb0a3);
    free(test->assemblyStr);
    free(test);     

    test = createDecodedInstruction(0x80000063);
    assert(strcmp(test->assemblyStr, "beq x0, x0, -4096") == 0);
    free(test->assemblyStr);
    free(test); 

    test = createDecodedInstruction(0x7fef8fe3);
    assert(strcmp(test->assemblyStr, "beq x31, x30, 4094") == 0);
    free(test->assemblyStr);
    free(test); 

    test = createEncodedInstruction("beq x0, x0, -4096");
    assert(test->input == 0x80000063);
    free(test->assemblyStr);
    free(test); 

    test = createEncodedInstruction("beq x31, x30, 4094");
    assert(test->input == 0x7fef8fe3);
    free(test->assemblyStr);
    free(test);  

    printf("\nPassed all tests\n");
}