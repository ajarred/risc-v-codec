#include "../include/encoder.h"

enum ErrorType {
    ERR_BUFFER_OVERFLOW,
    ERR_INSTR,
    ERR_INSTR_R,
    ERR_INSTR_I,
    ERR_INSTR_S,
    ERR_REGISTER_OVERFLOW,
    ERR_RD_X0
};

void printEncodeError(instruction*i, const enum ErrorType err) {
    i->type = INVALID;
    switch(err) {
    case ERR_BUFFER_OVERFLOW:
        fprintf(stderr, "Buffer overflow\n");
        break;
    case ERR_INSTR:
        fprintf(stderr, "Invalid instruction\n");
        break;
    case ERR_INSTR_R:
        fprintf(stderr, "Invalid R-type instruction input\n");
        break;
    case ERR_INSTR_I:
        fprintf(stderr, "Invalid I-type instruction input\n");
        break;
    case ERR_INSTR_S:
        fprintf(stderr, "Invalid I-type instruction input\n");
        break;
    case ERR_REGISTER_OVERFLOW:
        fprintf(stderr, "Invalid register is greater than 31\n");
        break;
    case ERR_RD_X0:
        fprintf(stderr, "Invalid rd: x0 cannot be modified\n");
        break;
    default:
        break;
    }
}

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
    printEncodeError(i, ERR_INSTR);
    return false;
}

void obtainArguments(instruction* i, const char* s) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    if (s == NULL || strlen(s) == 0) {
        return;
    }
    char instr[6] = "";
    char rs1[4] = "";
    char rs2[4] = "";
    char rd[4] = "";
    char imm[6] = "";
    switch (i->type) {
    case R:
        // format: add rd, rs1, rs2
        // format: sub rd, rs1, rs2
        if (sscanf(s, "%5s x%3[^,], x%3[^,], x%3s", instr, rd, rs1, rs2) == 4) {
            // printf("String: instr = %s, rd = %s, rs1 = %s, rs2 = %s\n", instr, rd, rs1, rs2);

            if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(rs2) > 3) {
                printEncodeError(i, ERR_BUFFER_OVERFLOW);
                return;
            }

            i->rd = (unsigned int)strtoul(rd, NULL, 10);
            i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
            i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);

            if (i->rd > 31 || i->rs1 > 31 || i->rs2 > 31) {
                printEncodeError(i, ERR_REGISTER_OVERFLOW);
                return;
            }
            if (i->rd == 0x0) {
                printEncodeError(i, ERR_RD_X0);
                return;
            }
            // printf("instr = %s, rd = x%d, rs1 = x%d, rs2 = x%d\n", i->instr, i->rd, i->rs1, i->rs2);
        } else {
            printEncodeError(i, ERR_INSTR_R);
            return;
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
                printEncodeError(i, ERR_INSTR_I);
                return;
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
                printEncodeError(i, ERR_INSTR_I);
                return;
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
                printEncodeError(i, ERR_INSTR_S);
                return;
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
        int immUpper = (i->immediate >> IMM_UPPER) & MASK_7BITS;
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