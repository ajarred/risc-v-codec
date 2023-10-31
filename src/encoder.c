#include "../include/encoder.h"

enum ErrorType {
    ERR_BUFFER_OVERFLOW,
    ERR_INSTR,
    ERR_INSTR_R,
    ERR_INSTR_I_IMM,
    ERR_INSTR_I_LOAD,
    ERR_INSTR_S,
    ERR_INSTR_B,
    ERR_REGISTER_OVERFLOW,
    ERR_IMM_OVERFLOW,
    ERR_RD_X0
};

void printEncodeError(instruction* i, const enum ErrorType err) {
    i->opcode = INVALID;
    switch(err) {
    case ERR_BUFFER_OVERFLOW:
        fprintf(stderr, "Buffer overflow\n");
        break;
    case ERR_INSTR:
        fprintf(stderr, "Invalid instruction\n");
        break;
    case ERR_INSTR_R:
        fprintf(stderr, "Invalid R-type instruction\nUsage: instr rd, rs1, rs2\n");
        break;
    case ERR_INSTR_I_IMM:
        fprintf(stderr, "Invalid I-type instruction\nUsage: instr rd, rs1, imm\n");
        break;
    case ERR_INSTR_I_LOAD:
        fprintf(stderr, "Invalid I-type instruction\nUsage: instr rd, imm(rs1)\n");
        break;
    case ERR_INSTR_S:
        fprintf(stderr, "Invalid S-type instruction\nUsage: instr rs2, imm(rs1)\n");
        break;
    case ERR_INSTR_B:
        fprintf(stderr, "Invalid B-type instruction\nUsage: instr rs1, rs2, imm\n");
        break;
    case ERR_REGISTER_OVERFLOW:
        fprintf(stderr, "Invalid register\n");
        break;
    case ERR_IMM_OVERFLOW:
        fprintf(stderr, "Invalid immediate. Not within valid range\n");
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
        strncmp(s, "sd",  2) == 0 ||
        strncmp(s, "beq", 3) == 0) {
        return true;
    }
    return false;
}

void obtainInstruction(instruction* i, const char* s) {
    if (!isValidInstruction(s) || i == NULL || 
                  s == NULL || strlen(s) == 0) {
        return;
    }
    if (strncmp(s, "addi",4) == 0) {
        strcpy(i->instr, "addi");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "add", 3) == 0) {
        strcpy(i->instr, "add");
        i->opcode = R_TYPE; 
        return;
    } else if (strncmp(s, "sub", 3) == 0) {
        strcpy(i->instr, "sub");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "ld",  2) == 0) {
        strcpy(i->instr, "ld");
        i->opcode = I_TYPE_LOAD;
        return;
    } else if (strncmp(s, "sd",  2) == 0) {
        strcpy(i->instr, "sd");
        i->opcode = S_TYPE;
        return;
    } else if (strncmp(s, "beq", 3) == 0) {
        strcpy(i->instr, "beq");
        i->opcode = B_TYPE;
        return;
    } else {
        printEncodeError(i, ERR_INSTR);
        return;
    }
}

void obtainArguments(instruction* i, const char* s) {
    if (i == NULL || i->opcode == INVALID || 
        s == NULL || strlen(s) == 0) {
        return;
    }
    i->assemblyStr = NULL;
    char instr[6] = "";
    char rs1[4] = "";
    char rs2[4] = "";
    char rd[4] = "";
    char imm[6] = "";
    switch (i->opcode) {
    case R_TYPE:
        // instr rd, rs1, rs2
        if (sscanf(s, "%5s x%3[^,], x%3[^,], x%3s", instr, rd, rs1, rs2) != 4) {
            printEncodeError(i, ERR_INSTR_R);
            return;
        } 
        if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(rs2) > 3) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        }
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
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
        break;
    case I_TYPE_IMM:
        // instr rd, rs1, imm
        if (sscanf(s, "%5s x%3[^,], x%3[^,], %5[-0-9]", instr, rd, rs1, imm) != 4) {
            printEncodeError(i, ERR_INSTR_R);
            return;
        }
        if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        }
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
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
        if (i->immediate < -2048 || i->immediate > 2047) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case I_TYPE_LOAD:
        // format: ld rd, imm(rs1)
        if (sscanf(s, "%5s x%3[^,], %5[-0-9](x%3[^)])", instr, rd, imm, rs1) != 4) {
            printEncodeError(i, ERR_INSTR_I_LOAD);
            return;
        }
        if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        }
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        strncpy(i->assemblyStr, s, strlen(s)+1);
        i->rd = (unsigned int)strtoul(rd, NULL, 10);
        i->immediate = (int)strtol(imm, NULL, 10);
        i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
        if (i->rd > 31 || i->rs1 > 31 || i->rs2 > 31) {
            printEncodeError(i, ERR_REGISTER_OVERFLOW);
            return;
        }
        if (i->rd == 0x0) {
            printEncodeError(i, ERR_RD_X0);
            return;
        }
        if (i->immediate < -2048 || i->immediate > 2047) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case S_TYPE:
        // instr rs2, imm(rs1)
        if (sscanf(s, "%5s x%3[^,], %5[-0-9](x%3[^)])", instr, rs2, imm, rs1) != 4) {
            printEncodeError(i, ERR_INSTR_S);
            return;
        }
        if (strlen(instr) > 5 || strlen(rs2) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        }
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        strncpy(i->assemblyStr, s, strlen(s)+1);
        i->rs2 = (unsigned int)strtoul(rs2, NULL, 10);
        i->immediate = (int)strtoul(imm, NULL, 10);
        i->rs1 = (unsigned int)strtol(rs1, NULL, 10);
        if (i->rs1 > 31 || i->rs2 > 31) {
            printEncodeError(i, ERR_REGISTER_OVERFLOW);
            return;
        }
        if (i->immediate < -2048 || i->immediate > 2047) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case B_TYPE:
        // format: beq rs1, rs2, imm
        if (sscanf(s, "%5s x%3[^,], x%3[^,], %5[-0-9]", instr, rs1, rs2, imm) != 4) {
            printEncodeError(i, ERR_INSTR_B);
            return;  
        }
        if (strlen(instr) > 5 || strlen(rs2) > 3 || strlen(rs1) > 3 || strlen(imm) > 5) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        }  
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        strncpy(i->assemblyStr, s, strlen(s)+1);
        i->rs1 = (unsigned int)strtoul(rs1, NULL, 10);
        i->rs2 = (unsigned int)strtol(rs2, NULL, 10);
        i->immediate = (int)strtoul(imm, NULL, 10);
        if (i->rs1 > 31 || i->rs2 > 31) {
            printEncodeError(i, ERR_REGISTER_OVERFLOW);
            return;
        }
        if (i->immediate < -4096 || i->immediate > 4094) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    default:
        break;
    }
}

void obtainFunct7(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    switch(i->opcode) {
    case R_TYPE:
        if (strncmp(i->instr, "add", 3) == 0) {
            i->funct7 = 0x0;
        }
        if (strncmp(i->instr, "sub", 3) == 0) {
            i->funct7 = 0x20;
        } 
        break;
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
    case S_TYPE:
    case B_TYPE:
    default:
        break;
    }
}

void obtainFunct3(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    if ((strncmp(i->instr, "add", 3) == 0) || 
        (strncmp(i->instr, "sub", 3) == 0) ||
        (strncmp(i->instr, "addi",4)== 0) ||
        (strncmp(i->instr, "beq", 3) == 0)) {
        i->funct3 = 0x0;
    } else if ((strncmp(i->instr, "ld", 2) == 0) ||
               (strncmp(i->instr, "sd", 2) == 0)) {
        i->funct3 = 0x3;
    } 
}

void obtainOpcode(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    switch(i->opcode) {
    case R_TYPE:
        if ((strncmp(i->instr, "add", 3) == 0) || 
        (strncmp(i->instr, "sub", 3) == 0)) {
            i->opcode = R_TYPE;
        }
        break;
    case I_TYPE_IMM:
        if (strncmp(i->instr, "addi", 4) == 0) {
            i->opcode = I_TYPE_IMM;
        }
        break;
    case I_TYPE_LOAD:
        if (strncmp(i->instr, "ld", 2) == 0) {
            i->opcode = I_TYPE_LOAD;
        }
        break;
    case S_TYPE:
        if (strncmp(i->instr, "sd", 2) == 0) {
            i->opcode = S_TYPE;
        }
        break;
    case B_TYPE:
        if (strncmp(i->instr, "beq", 2) == 0) {
            i->opcode = B_TYPE;
        }
        break; 
    default:
        break;
    }
}

void obtainInput(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int immLower;
    int immUpper;
    switch(i->opcode) {
    case R_TYPE:
        i->input = i->opcode | (i->rd << BIT_RD) | 
            (i->funct3 << BIT_FUNCT3) | 
            (i->rs1 << BIT_RS1) |
            (i->rs2 << BIT_RS2) |
            (i->funct7 << BIT_FUNCT7);
        break;
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
        i->input = i->opcode | (i->rd << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->rs1 << BIT_RS1) |
            (i->immediate << BIT_RS2);
        break;
    case S_TYPE:
        immLower = i->immediate & MASK_5BITS; 
        immUpper = (i->immediate >> IMM_UPPER) & MASK_7BITS;
        i->input = i->opcode | (immLower << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->rs1 << BIT_RS1) |
            (i->rs2 << BIT_RS2) |
            (immUpper << BIT_FUNCT7);
        break;
    case B_TYPE:
        immUpper = (((i->immediate >> 12) & MASK_1BIT) << 6) |
                   ((i->immediate >> 5) & MASK_6BITS); 
        immLower = (((i->immediate >> 2) & MASK_4BITS) << 1) |
                   ((i->immediate >> 1) & MASK_1BIT);
        i->input = i->opcode | (immLower << BIT_RD) |
                  (i->funct3 << BIT_FUNCT3) |
                  (i->rs1 << BIT_RS1) |
                  (i->rs2 << BIT_RS2) |
                  (immUpper << BIT_FUNCT7);
        break;
    default:
        break;
    }
}

instruction* createEncodedInstruction(const char* s) {
    instruction* i = malloc(sizeof(instruction));
    obtainInstruction(i, s);
    obtainArguments(i, s);
    obtainFunct7(i);
    obtainFunct3(i);
    obtainOpcode(i);
    obtainInput(i);
    if (i->opcode == INVALID) {
        return NULL;
    }
    return i;
}

bool encodeInstruction(const char* s) {
    instruction* i = createEncodedInstruction(s);
    if (i == NULL) {
        return false;
    }
    printf("0x%x\n", i->input);
    free(i->assemblyStr);
    free(i);
    return true;
}