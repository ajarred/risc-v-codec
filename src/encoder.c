#include "../include/encoder.h"

bool isValidInstruction(const char* s) {
    if (strncmp(s, "add", 3) == 0 ||
        strncmp(s, "sub", 3) == 0 ||
        strncmp(s, "addi",4) == 0 ||
        strncmp(s, "ld",  2) == 0 || 
        strncmp(s, "sd",  2) == 0) {
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
    i->type = INVALID;
    return false;
}

void obtainArguments(instruction* i, const char* s) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    if (s == NULL || strlen(s) == 0) {
        return;
    }
    char instr[5] = "";
    char rs1[5] = "";
    char rs2[5] = "";
    char rd[5] = "";
    char imm[5] = "";
    switch (i->type) {
    case R:
        // format: add rd, rs1, rs2
        // format: sub rd, rs1, rs2
        if (sscanf(s, "%s x%[^,], x%[^,], x%s", instr, rd, rs1, rs2) == 4) {
            // printf("String: instr = %s, rd = %s, rs1 = %s, rs2 = %s\n", instr, rd, rs1, rs2);
            i->rd = (unsigned int)strtoul(rd, NULL, 10);
            i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
            i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);
            // printf("instr = %s, rd = x%d, rs1 = x%d, rs2 = x%d\n", i->instr, i->rd, i->rs1, i->rs2);
        } else {
            fprintf(stderr, "Invalid R-type instruction input\n");
            printf("instr = %s, rd = %s, rs1 = %s, rs2 = %s\n", instr, rd, rs1, rs2);
            i->type = INVALID; 
        } 
        break;
    case I:
        // format: addi rd, rs1, imm
        if (strcmp(i->instr, "addi") == 0) {
            if (sscanf(s, "%s x%[^,], x%[^,], %[-0-9]", instr, rd, rs1, imm) == 4) {
                // printf("String: instr = %s, rd = %s, rs1 = %s, imm = %s\n", instr, rd, rs1, imm);
                i->rd = (unsigned int)strtoul(rd, NULL, 10);
                i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
                i->immediate = (int)strtol(imm, NULL, 10);
                // printf("instr = %s, rd = x%d, rs1 = x%d, imm = %d\n", i->instr, i->rd, i->rs1, i->immediate);
            } else {
                fprintf(stderr, "Invalid I-type instruction input\n");
                printf("instr = %s, rd = %s, rs1 = %s, imm = %s\n", instr, rd, rs1, imm);
                i->type = INVALID; 
            }
        }
        // format: ld rd, imm (rs1)
        if (strcmp(i->instr, "ld") == 0) {
            if (sscanf(s, "%s x%[^,], %[-0-9] ( x%[^)] )", instr, rd, imm, rs1) == 4) {
                // printf("String: instr = %s, rd = %s, rs1 = %s, imm = %s\n", instr, rd, rs1, imm);
                i->rd = (unsigned int)strtoul(rd, NULL, 10);
                i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
                i->immediate = (int)strtol(imm, NULL, 10);
                // printf("instr = %s, rd = x%d, rs1 = x%d, imm = %d\n", i->instr, i->rd, i->rs1, i->immediate);
            } else {
                fprintf(stderr, "Invalid I-type instruction input\n");
                printf("instr = %s, rd = %s, rs1 = %s, imm = %s\n", instr, rd, rs1, imm);
                i->type = INVALID; 
            } 
        }
        break;
    case S:
        // format: sd rs2, imm (rs1)
        if (strcmp(i->instr, "sd") == 0) {
            if (sscanf(s, "%s x%[^,], %[-0-9] ( x%[^)] )", instr, rs2, imm, rs1) == 4) {
                printf("String: instr = %s, rs2 = %s, imm = %s, rs1 = %s\n", instr, rs2, imm, rs1);
                i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);
                i->immediate = (int)strtoul(imm, NULL, 10);
                i->rs1 = (unsigned int)strtol(rs1, NULL, 10);
                printf("instr = %s, rs2 = x%d, imm = %d, rs1 = x%d\n", i->instr, i->rs2, i->immediate, i->rs1);
            } else {
                fprintf(stderr, "Invalid I-type instruction input\n");
                printf("instr = %s, rs2 = %s, imm = %s, rs1 = %s\n", instr, rs2, imm, rs1);
                i->type = INVALID; 
            } 
        }
        break;
    default:
        break;
    }
}

void obtainFunct7(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    switch(i->type) {
    case R:
        if (strcmp(i->instr, "add") == 0) {
            i->funct7 = 0x0;
            // printf("funct7 = %x\n", i->funct7);
        }
        if (strcmp(i->instr, "sub") == 0) {
            i->funct7 = 0x20;
            // printf("funct7 = %x\n", i->funct7);
        } 
        break;
    case I:
    case S:
    default:
        break;
    }
}

// funct3
void obtainFunct3(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }

}

// opcode
void obtainOpcode(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }

}

// input
void obtainInput(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
}