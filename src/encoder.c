#include "../include/encoder.h"

bool isValidInstruction(const char* s) {
    if (strncmp(s, "add", 3) == 0 ||
        strncmp(s, "sub", 3) == 0 ||
        strncmp(s, "addi",4) == 0 ||
        strncmp(s, "ld",  4) == 0 || 
        strncmp(s, "sd",  4) == 0) {
        return true;
    }
    return false;
}

void obtainInstruction(instruction* i, const char* s) {
    if (i == NULL) {
        return;
    }
    if (s == NULL || strlen(s) == 0) {
        fprintf(stderr, "Invalid string input\n");
        return;
    }
    if (strncmp(s, "addi",4) == 0) {
        strcpy(i->instr, "addi");
        i->type = I;
        return;
    }
    if (strncmp(s, "add", 3) == 0) {
        strcpy(i->instr, "add");
        i->type = R; 
        return;
    }
    if (strncmp(s, "sub", 3) == 0) {
        strcpy(i->instr, "sub");
        i->type = R;
        return;
    }
    if (strncmp(s, "ld",  2) == 0) {
        strcpy(i->instr, "ld");
        i->type = I;
        return;
    }
    if (strncmp(s, "sd",  2) == 0) {
        strcpy(i->instr, "sd");
        i->type = S;
        return;
    }
    return;
}

void obtainRegisters(instruction*i, const char* s) {
    char instr[5] = "";
    char rs1[5] = "";
    char rs2[5] = "";
    char rd[5] = "";
    if (sscanf(s, "%4s %4[^,], %4[^,], %4s", instr, rs1, rs2, rd) == 4) {
        printf("instr = %s, rd = %s, rs1 = %s, rs1 = %s\n", instr, rd, rs1, rs2);
    } else {
        fprintf(stderr, "Invalid assembly instruction input\n");
        printf("instr = %s, rd = %s, rs1 = %s, rs2 = %s\n", instr, rd, rs1, rs2);
    } 
}