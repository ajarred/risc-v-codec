#include "../include/encoder.h"

enum ErrorType {
    ERR_BUFFER_OVERFLOW,
    ERR_INSTR,
    ERR_INSTR_R,
    ERR_INSTR_I,
    ERR_INSTR_S,
    ERR_REGISTER_OVERFLOW,
    ERR_RD_X0,
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
        fprintf(stderr, "Invalid S-type instruction input\n");
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

void obtainInstruction(instruction* i, const char* s) {
    if (!isValidInstruction(s)) {
        return;
    }
    if (i == NULL) {
        return;
    }
    if (s == NULL || strlen(s) == 0) {
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
    printEncodeError(i, ERR_INSTR);
    return;
}

void obtainArguments(instruction* i, const char* s) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    if (s == NULL || strlen(s) == 0) {
        return;
    }
    i->assemblyStr = NULL;
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
            if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(rs2) > 3) {
                printEncodeError(i, ERR_BUFFER_OVERFLOW);
                return;
            }
            i->assemblyStr = (char*) malloc(strlen(s)+1);
            strncpy(i->assemblyStr, s, strlen(s)+1);
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
        } else {
            printEncodeError(i, ERR_INSTR_R);
            return;
        } 
        break;
    case I:
        // format: addi rd, rs1, imm
        if (strcmp(i->instr, "addi") == 0) {
            if (sscanf(s, "%5s x%3[^,], x%3[^,], %5[-0-9]", instr, rd, rs1, imm) == 4) {
                if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
                    printEncodeError(i, ERR_BUFFER_OVERFLOW);
                    return;
                }
                i->assemblyStr = (char*) malloc(strlen(s)+1);
                strncpy(i->assemblyStr, s, strlen(s)+1);
                i->rd = (unsigned int)strtoul(rd, NULL, 10);
                i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
                i->immediate = (int)strtol(imm, NULL, 10);

                if (i->rd > 31 || i->rs1 > 31) {
                    printEncodeError(i, ERR_REGISTER_OVERFLOW);
                    return;
                }
                if (i->rd == 0x0) {
                    printEncodeError(i, ERR_RD_X0);
                    return;
                }
            } else {
                printEncodeError(i, ERR_INSTR_I);
                return;
            }
        }
        // format: ld rd, imm (rs1)
        if (strcmp(i->instr, "ld") == 0) {
            if (sscanf(s, "%5s x%3[^,], %5[-0-9] ( x%3[^)] )", instr, rd, imm, rs1) == 4) {
                if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
                    printEncodeError(i, ERR_BUFFER_OVERFLOW);
                    return;
                }
                i->assemblyStr = (char*) malloc(strlen(s)+1);
                strncpy(i->assemblyStr, s, strlen(s)+1);
                i->rd = (unsigned int)strtoul(rd, NULL, 10);
                i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
                i->immediate = (int)strtol(imm, NULL, 10);

                if (i->rd > 31 || i->rs1 > 31 || i->rs2 > 31) {
                    printEncodeError(i, ERR_REGISTER_OVERFLOW);
                    return;
                }
                if (i->rd == 0x0) {
                    printEncodeError(i, ERR_RD_X0);
                    return;
                }
            } else {
                printEncodeError(i, ERR_INSTR_I);
                return;
            } 
        }
        break;
    case S:
        // format: sd rs2, imm (rs1)
        if (strcmp(i->instr, "sd") == 0) {
            if (sscanf(s, "%5s x%3[^,], %5[-0-9] ( x%3[^)] )", instr, rs2, imm, rs1) == 4) {
                if (strlen(instr) > 5 || strlen(rs2) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
                    printEncodeError(i, ERR_BUFFER_OVERFLOW);
                    return;
                }
                i->assemblyStr = (char*) malloc(strlen(s)+1);
                strncpy(i->assemblyStr, s, strlen(s)+1);
                i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);
                i->immediate = (int)strtoul(imm, NULL, 10);
                i->rs1 = (unsigned int)strtol(rs1, NULL, 10);
                if (i->rs1 > 31 || i->rs2 > 31) {
                    printEncodeError(i, ERR_REGISTER_OVERFLOW);
                    return;
                }
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
        }
        if (strcmp(i->instr, "sub") == 0) {
            i->funct7 = 0x20;
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
        } 
    if ((strcmp(i->instr, "ld") == 0) ||
        (strcmp(i->instr, "sd") == 0)) {
            i->funct3 = 0x3;
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
        break;
    case I:
        if (strcmp(i->instr, "addi") == 0) {
            i->opcode = 0x13;
        }
        if (strcmp(i->instr, "ld") == 0) {
            i->opcode = 0x3;
        }
        break;
    case S:
        if (strcmp(i->instr, "sd") == 0) {
            i->opcode = 0x23;
        }
        break;
    default:
        break;
    }
}

void obtainInput(instruction* i) {
    if (i == NULL || i->type == INVALID) {
        return;
    }
    unsigned int immLower;
    int immUpper;
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
        immLower = i->immediate & MASK_5BITS; 
        immUpper = (i->immediate >> IMM_UPPER) & MASK_7BITS;
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

bool encodeInstruction(const char* s) {
    instruction* i = malloc(sizeof(instruction));
    obtainInstruction(i, s);
    obtainArguments(i, s);
    obtainFunct7(i);
    obtainFunct3(i);
    obtainOpcode(i);
    if (i == NULL) {
        return false;
    }
    obtainInput(i);
    free(i->assemblyStr);
    free(i);
    return true;
}