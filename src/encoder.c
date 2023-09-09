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
                // printf("String: instr = %s, rs2 = %s, imm = %s, rs1 = %s\n", instr, rs2, imm, rs1);
                i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);
                i->immediate = (int)strtoul(imm, NULL, 10);
                i->rs1 = (unsigned int)strtol(rs1, NULL, 10);
                // printf("instr = %s, rs2 = x%d, imm = %d, rs1 = x%d\n", i->instr, i->rs2, i->immediate, i->rs1);
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

void obtainFunct3(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    if ((strcmp(i->instr, "add") == 0) || 
        (strcmp(i->instr, "sub") == 0) ||
        (strcmp(i->instr, "addi")== 0)) {
            i->funct3 = 0x0;
            // printf("funct3 = %x\n", i->funct3);
        } 
    if ((strcmp(i->instr, "ld") == 0) ||
        (strcmp(i->instr, "sd") == 0)) {
            i->funct3 = 0x3;
            // printf("funct3 = %x\n", i->funct3);
        } 
}

void obtainOpcode(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    switch(i->type) {
    case R:
        if ((strcmp(i->instr, "add") == 0) || 
        (strcmp(i->instr, "sub") == 0)) {
            i->opcode = 0x33;
        }
        // printf("opcode = %x\n", i->opcode);
        break;
    case I:
        if (strcmp(i->instr, "addi") == 0) {
            i->opcode = 0x13;
        }
        if (strcmp(i->instr, "ld") == 0) {
            i->opcode = 0x3;
        }
        // printf("opcode = %x\n", i->opcode);
        break;
    case S:
        if (strcmp(i->instr, "sd") == 0) {
            i->opcode = 0x23;
        }
        // printf("opcode = %x\n", i->opcode);
        break;
    default:
        break;
    }
}

// input
void obtainInput(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    switch(i->type) {
    case R:
        i->input = i->opcode | (i->rd << BIT_RD) | 
            (i->funct3 << BIT_FUNCT3) | 
            (i->rs1 << BIT_RS1) |
            (i->rs2 << BIT_RS2) |
            (i->funct7 << BIT_FUNCT7);
        printf("input = 0x%x\n", i->input);
        break;
    case I:
        i->input = i->opcode | (i->rd << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->rs1 << BIT_RS1) |
            (i->immediate << BIT_RS2);
        printf("input = 0x%x\n", i->input);
        break;
    case S:
        unsigned int immLower = i->immediate & MASK_5BITS; 
        int immUpper = i->immediate & (MASK_7BITS << IMM_UPPER);
        printf("lower imm = %u\n", immLower);
        printf("upper imm = %u\n", immUpper);
        i->input = i->opcode | (immLower << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->rs1 << BIT_RS1) |
            (i->rs2 << BIT_RS2) |
            (immUpper << BIT_FUNCT7);
        printf("input = 0x%x\n", i->input);
        break;
    default:
        break;
    }
}

void encodeInstructions(instruction* i, const char* s) {
    obtainArguments(i, s);
    obtainFunct7(i);
    obtainFunct3(i);
    obtainOpcode(i);
    obtainInput(i);
}