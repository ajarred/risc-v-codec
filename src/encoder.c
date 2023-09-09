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

bool obtainInstruction(instruction* i, const char* s) {
    if (i == NULL) {
        return false;
    }
    if (s == NULL || strlen(s) == 0) {
        fprintf(stderr, "Invalid string input\n");
        return false;
    }
    if (strncmp(s, "addi",4) == 0) {
        strcpy(i->instr, "addi");
        i->type = I;
        return true;
    }
    if (strncmp(s, "add", 3) == 0) {
        strcpy(i->instr, "add");
        i->type = R; 
        return true;
    }
    if (strncmp(s, "sub", 3) == 0) {
        strcpy(i->instr, "sub");
        i->type = R;
        return true;
    }
    if (strncmp(s, "ld",  2) == 0) {
        strcpy(i->instr, "ld");
        i->type = I;
        return true;
    }
    if (strncmp(s, "sd",  2) == 0) {
        strcpy(i->instr, "sd");
        i->type = S;
        return true;
    }
    return false;
}

void obtainArguments(instruction* i, const char* s) {
    char instr[5] = "";
    char rs1[5] = "";
    char rs2[5] = "";
    char rd[5] = "";
    // char imm[5] = "";
    
    switch (i->type) {
    case R:
        if (sscanf(s, "%4s x%4[^,], x%4[^,], x%4s", instr, rd, rs1, rs2) == 4) {
            // printf("String: instr = %s, rd = %s, rs1 = %s, rs1 = %s\n", instr, rd, rs1, rs2);
            i->rd = (unsigned int)strtol(rd, NULL, 10);
            i->rs1 = (unsigned int)strtol(rs1, NULL, 10);
            i->rs2 = (unsigned int)strtol(rs2, NULL, 10);
            printf("instr = %s, rd = x%d, rs1 = x%d, rs1 = x%d\n", i->instr, i->rd, i->rs1, i->rs2);
        } else {
            fprintf(stderr, "Invalid R-type instruction input\n");
            printf("instr = %s, rd = %s, rs1 = %s, rs2 = %s\n", instr, rd, rs1, rs2);
        } 
        break;
    case I:
        break;
    case S:
        break;
    }
}