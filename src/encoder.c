#include "../include/encoder.h"
#include <ctype.h>

enum ErrorType {
    ERR_BUFFER_OVERFLOW,
    ERR_INSTR,
    ERR_INSTR_R,
    ERR_INSTR_I_IMM,
    ERR_INSTR_I_LOAD,
    ERR_INSTR_S,
    ERR_INSTR_B,
    ERR_INSTR_J,
    ERR_INSTR_U,
    ERR_INSTR_ENV,
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
    case ERR_INSTR_J:
        fprintf(stderr, "Invalid J-type instruction\nUsage: instr rd imm\n");
        break;
    case ERR_INSTR_U:
        fprintf(stderr, "Invalid U-type instruction\nUsage: instr rd imm\n");
        break;
    case ERR_INSTR_ENV:
        fprintf(stderr, "Invalid Environment instruction\nUsage: instr\n");
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

char* convertToLower(const char *str) {
    int length = strlen(str);
    char *lowercaseStr = (char*)malloc(length + 1);
    if (lowercaseStr == NULL) {
        return NULL;
    }
    for (int i = 0; i < length; i++) {
        lowercaseStr[i] = tolower((unsigned char)str[i]);
    }
    lowercaseStr[length] = '\0';
    return lowercaseStr;
}

bool isValidInstruction(const char* str) {
    char* s = convertToLower(str);
    if (strncmp(s, "add", 3) == 0 ||
        strncmp(s, "sub", 3) == 0 ||
        strncmp(s, "xor", 3) == 0 ||
        strncmp(s, "or",  2) == 0 ||
        strncmp(s, "and", 3) == 0 ||
        strncmp(s, "sltiu",5)== 0 ||
        strncmp(s, "slti",4) == 0 ||
        strncmp(s, "slli",4) == 0 ||
        strncmp(s, "srli",4) == 0 ||
        strncmp(s, "srai",4) == 0 ||
        strncmp(s, "sll", 3) == 0 ||
        strncmp(s, "srl", 3) == 0 ||
        strncmp(s, "sra", 3) == 0 ||
        strncmp(s, "slt", 3) == 0 ||
        strncmp(s, "sltu",4) == 0 ||
        strncmp(s, "addi",4) == 0 ||
        strncmp(s, "xori",4) == 0 ||
        strncmp(s, "ori", 3) == 0 ||
        strncmp(s, "andi",4) == 0 ||
        strncmp(s, "ld",  2) == 0 || 
        strncmp(s, "sd",  2) == 0 ||
        strncmp(s, "beq", 3) == 0 ||
        strncmp(s, "bne", 3) == 0 ||
        strncmp(s, "blt", 3) == 0 ||
        strncmp(s, "bge", 3) == 0 ||
        strncmp(s, "bgeu",4) == 0 ||
        strncmp(s, "bltu",4) == 0 ||
        strncmp(s, "jal", 3) == 0 ||
        strncmp(s,"jalr", 4) == 0 ||
        strncmp(s, "lui", 3) == 0 ||
        strncmp(s,"auipc",5) == 0 ||
        strncmp(s,"ecall",4) == 0 ||
        strncmp(s,"ebreak",6) == 0) {
        return true;
    }
    return false;
}

void obtainOpcode(instruction* i, const char* str) {
    if (!isValidInstruction(str) || i == NULL || str == NULL || strlen(str) == 0) {
        return;
    }
    char* s = convertToLower(str);
    if (strncmp(s, "ebreak", 6) == 0) {
        strcpy(i->instr, "ebreak");
        i->opcode = ENV;
        return;
    } else if (strncmp(s, "ecall", 5) == 0) {
        strcpy(i->instr, "ecall");
        i->opcode = ENV;
        return;
    } else if (strncmp(s, "auipc", 5) == 0) {
        strcpy(i->instr, "auipc");
        i->opcode = AUIPC;
        return;
    } else if (strncmp(s, "sltiu", 5) == 0) {
        strcpy(i->instr, "sltiu");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "slti", 4) == 0) {
        strcpy(i->instr, "slti");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "addi", 4) == 0) {
        strcpy(i->instr, "addi");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "sltu", 4) == 0) {
        strcpy(i->instr, "sltu");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "slli", 4) == 0) {
        strcpy(i->instr, "slli");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "srli", 4) == 0) {
        strcpy(i->instr, "srli");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "srai", 4) == 0) {
        strcpy(i->instr, "srai");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "xori", 4) == 0) {
        strcpy(i->instr, "xori");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "andi", 4) == 0) {
        strcpy(i->instr, "andi");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "bgeu", 4) == 0) {
        strcpy(i->instr, "bgeu");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "bltu", 4) == 0) {
        strcpy(i->instr, "bltu");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "jalr", 4) == 0) {
        strcpy(i->instr, "jalr");
        i->opcode = JALR;
        return;
    } else if (strncmp(s, "ori", 3) == 0) {
        strcpy(i->instr, "ori");
        i->opcode = I_TYPE_IMM;
        return;
    } else if (strncmp(s, "blt", 3) == 0) {
        strcpy(i->instr, "blt");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "bne", 3) == 0) {
        strcpy(i->instr, "bne");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "bge", 3) == 0) {
        strcpy(i->instr, "bge");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "lui", 3) == 0) {
        strcpy(i->instr, "lui");
        i->opcode = LUI;
        return;
    } else if (strncmp(s, "add", 3) == 0) {
        strcpy(i->instr, "add");
        i->opcode = R_TYPE; 
        return;
    } else if (strncmp(s, "sub", 3) == 0) {
        strcpy(i->instr, "sub");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "xor", 3) == 0) {
        strcpy(i->instr, "xor");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "and", 3) == 0) {
        strcpy(i->instr, "and");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "sll", 3) == 0) {
        strcpy(i->instr, "sll");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "srl", 3) == 0) {
        strcpy(i->instr, "srl");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "sra", 3) == 0) {
        strcpy(i->instr, "sra");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "slt", 3) == 0) {
        strcpy(i->instr, "slt");
        i->opcode = R_TYPE;
        return;
    } else if (strncmp(s, "beq", 3) == 0) {
        strcpy(i->instr, "beq");
        i->opcode = B_TYPE;
        return;
    } else if (strncmp(s, "jal", 3) == 0) {
        strcpy(i->instr, "jal");
        i->opcode = J_TYPE;
        return;
    } else if (strncmp(s, "ld",  2) == 0) {
        strcpy(i->instr, "ld");
        i->opcode = I_TYPE_LOAD;
        return;
    } else if (strncmp(s, "sd",  2) == 0) {
        strcpy(i->instr, "sd");
        i->opcode = S_TYPE;
        return;
    } else if (strncmp(s, "or", 2) == 0) {
        strcpy(i->instr, "or");
        i->opcode = R_TYPE;
        return;
    } else {
        printEncodeError(i, ERR_INSTR);
        return;
    }
}

void obtainArguments(instruction* i, const char* str) {
    if (i == NULL || i->opcode == INVALID || str == NULL || strlen(str) == 0) {
        return;
    }
    char* s = convertToLower(str);
    i->assemblyStr = NULL;
    char instr[7] = "";
    char rs1[4] = "";
    char rs2[4] = "";
    char rd[4] = "";
    char imm[9] = "";
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
        if (strncmp(i->instr, "slli", 4) == 0 || 
            strncmp(i->instr, "srli", 4) == 0 ||
            strncmp(i->instr, "srai", 4) == 0) {
            if (i->immediate < 0 || i->immediate > 31) {
                printEncodeError(i, ERR_IMM_OVERFLOW);
                return;  
            }
        }
        if (i->immediate < -2048 || i->immediate > 2047) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case I_TYPE_LOAD:
    case JALR:
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
        // instr rs1, rs2, imm
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
    case J_TYPE:
        // instr rd, imm
        if (sscanf(s, "%5s x%3[^,], %8[-0-9]", instr, rd, imm) != 3) {
            printEncodeError(i, ERR_INSTR_U);
            return;  
        }
        if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(imm) > 8) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        } 
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        strncpy(i->assemblyStr, s, strlen(s)+1);
        i->rd = (unsigned int)strtoul(rd, NULL, 10);
        i->immediate = (int)strtoul(imm, NULL, 10);
        if (i->rd > 31) {
            printEncodeError(i, ERR_REGISTER_OVERFLOW);
            return;
        }
        if (i->immediate < -1048576 || i->immediate > 1048574) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case LUI:
    case AUIPC:
        // instr rd, imm
        if (sscanf(s, "%5s x%3[^,], %8[-0-9]", instr, rd, imm) != 3) {
            printEncodeError(i, ERR_INSTR_J);
            return;  
        }
        if (strlen(instr) > 5 || strlen(rd) > 3 || strlen(imm) > 8) {
            printEncodeError(i, ERR_BUFFER_OVERFLOW);
            return;
        } 
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        strncpy(i->assemblyStr, s, strlen(s)+1);
        i->rd = (unsigned int)strtoul(rd, NULL, 10);
        i->immediate = (int)strtoul(imm, NULL, 10);
        if (i->rd > 31) {
            printEncodeError(i, ERR_REGISTER_OVERFLOW);
            return;
        }
        if (i->immediate < -524288 || i->immediate > 524287) {
            printEncodeError(i, ERR_IMM_OVERFLOW);
            return;
        }
        break;
    case ENV:
        // instr
        if (sscanf(s, "%7s", instr) != 1) {
            printEncodeError(i, ERR_INSTR_ENV);
            return;  
        }
        if (strlen(instr) > 7) {
            printEncodeError(i, ERR_INSTR_ENV);
            return;
        } 
        i->assemblyStr = (char*) malloc(strlen(s)+1);
        if (i->assemblyStr == NULL) {
            return;
        }
        i->rs1 = 0x0;
        i->rd = 0x0;
        strncpy(i->assemblyStr, s, strlen(s)+1);
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
        if (strncmp(i->instr, "or",  2) == 0 ||
            strncmp(i->instr, "add", 3) == 0 ||
            strncmp(i->instr, "xor", 3) == 0 ||
            strncmp(i->instr, "and", 3) == 0 ||
            strncmp(i->instr, "sll", 3) == 0 ||
            strncmp(i->instr, "srl", 3) == 0 ||
            strncmp(i->instr, "slt", 3) == 0 ||
            strncmp(i->instr, "sltu",4) == 0) {
            i->funct7 = 0x0;
        }
        if (strncmp(i->instr, "sub", 3) == 0 || 
            strncmp(i->instr, "sra", 3) == 0) {
            i->funct7 = 0x20;
        } 
        break;
    case ENV:
        if (strncmp(i->instr, "ecall", 5) == 0) {
            i->immediate = 0x0;
        } else if (strncmp(i->instr, "ebreak", 6) == 0) {
            i->immediate = 0x1;
        } else {
            printEncodeError(i, ERR_INSTR_ENV);
        }
        break;
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
        (strncmp(i->instr, "addi",4) == 0) ||
        (strncmp(i->instr, "beq", 3) == 0) ||
        (strncmp(i->instr, "jalr",4) == 0) ||
        (strncmp(i->instr, "ecall", 5) == 0) ||
        (strncmp(i->instr, "ebreak", 6) == 0)) {
        i->funct3 = 0x0;
    } else if ((strncmp(i->instr, "ld", 2) == 0) ||
               (strncmp(i->instr, "sd", 2) == 0)) {
        i->funct3 = 0x3;
    } else if (strncmp(i->instr, "or", 2) == 0 ||
              (strncmp(i->instr, "ori", 3)== 0)||
              (strncmp(i->instr, "bltu",4) == 0)) {
        i->funct3 = 0x6;
    } else if (strncmp(i->instr, "xor", 3) == 0 ||
              (strncmp(i->instr, "xori", 4)== 0)||
              (strncmp(i->instr, "blt", 3) == 0)) {
        i->funct3 = 0x4;
    } else if (strncmp(i->instr, "and", 3) == 0 || 
              (strncmp(i->instr, "andi", 4)== 0)||
              (strncmp(i->instr, "bgeu", 4)== 0)) {
        i->funct3 = 0x7;
    } else if (strncmp(i->instr, "slli",4) == 0 ||
               strncmp(i->instr, "sll", 3) == 0 ||
               strncmp(i->instr, "bne", 3) == 0) {
        i->funct3 = 0x1;
    } else if (strncmp(i->instr, "srli",4) == 0 ||
               strncmp(i->instr, "srai",4) == 0 ||
               strncmp(i->instr, "srl", 3) == 0 ||
               strncmp(i->instr, "sra", 3) == 0 ||
               strncmp(i->instr, "bge", 3) == 0) {
        i->funct3 = 0x5;
    } else if (strncmp(i->instr, "sltu", 4) == 0 ||
               strncmp(i->instr, "sltiu",5) == 0) {
        i->funct3 = 0x3;
    } else if (strncmp(i->instr, "slt", 3) == 0 || 
               strncmp(i->instr, "slti",4) == 0) {
        i->funct3 = 0x2;
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
        switch (i->funct3) {
        case 0x1:
            i->input = i->opcode | (i->rd << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->immediate << BIT_RS2); 
            break;
        case 0x5:
            if (strncmp(i->instr, "srli", 4) == 0) {
                i->input = i->opcode | (i->rd << BIT_RD) |
                (i->funct3 << BIT_FUNCT3) |
                (i->rs1 << BIT_RS1) |
                (i->immediate << BIT_RS2) | 
                (0x0 << BIT_FUNCT7);
            } else if (strncmp(i->instr, "srai", 4) == 0) {
                i->input = i->opcode | (i->rd << BIT_RD) |
                (i->funct3 << BIT_FUNCT3) |
                (i->rs1 << BIT_RS1) |
                (i->immediate << BIT_RS2) |
                (0x20 << BIT_FUNCT7);
            }
            break;
        default:
            i->input = i->opcode | (i->rd << BIT_RD) |
            (i->funct3 << BIT_FUNCT3) |
            (i->rs1 << BIT_RS1) |
            (i->immediate << BIT_RS2);
            break;
        }
        break;
    case I_TYPE_LOAD:
    case JALR:
    case ENV:
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
    case J_TYPE:
        // imm[20|10:1|11|19:12  
        immUpper = (((i->immediate >> 12) & MASK_8BITS) << 1) | // 19:12
        (((i->immediate >> 11) & MASK_1BIT) << 9) | // 11 
        (((i->immediate >> 1) & MASK_10BITS) << 10) | // 10:1 
        (((i->immediate >> 20) & MASK_1BIT) << 20); // 20         
        i->input = ((immUpper >> 1) << BIT_FUNCT3) | 
                   (i->rd << BIT_RD) |
                   i->opcode;
        break;
    case LUI:
    case AUIPC:
        i->input = (i->immediate << BIT_FUNCT3) | 
                   (i->rd << BIT_RD) | i->opcode;
        break;
    default:
        break;
    }
}

instruction* createEncodedInstruction(const char* s) {
    instruction* i = malloc(sizeof(instruction));
    obtainOpcode(i, s);
    obtainArguments(i, s);
    obtainFunct7(i);
    obtainFunct3(i);
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