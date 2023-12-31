#include "../include/decoder.h"

enum ErrorType {
    ERR_OPCODE,
    ERR_FUNCT3,
    ERR_IMM,
    ERR_FUNCT7,
    ERR_X0_RD,
    ERR_ENV
};

bool isHex(char* str) {
    size_t length = strlen(str);
    if (length > 0 && str[length - 1] == '\n') {
        str[length - 1] = '\0';
        length--;
    }
    if (length <= 10 && strncmp(str, "0x", 2) == 0) {
        for (size_t i = 2; i < length; i++) {
            if (!isxdigit((unsigned char)str[i])) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool convertStrToUint(char* str, unsigned int *n) {
    if(!isHex(str))
    {
        fprintf(stderr, "Invalid input. Please input a valid hex string\n");
        printf("String length = %ld\n", strlen(str));
        return false;
    }
    *n = (unsigned int)strtol(str, NULL, 16);
    return true;
}

void printDecodeError(instruction* i, const enum ErrorType err) {
    i->opcode = INVALID;
    switch (err) {
    case ERR_OPCODE:
        fprintf(stderr, "Opcode not found\n");
        printf("Opcode = %x\n", i->opcode);
        break;
    case ERR_FUNCT3:
        fprintf(stderr, "Invalid funct3\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x\n", i->funct3);
        break;
    case ERR_IMM:
        fprintf(stderr, "Invalid immediate\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);
        printf("Immediate = %x\n", i->immediate);
        break;
    case ERR_FUNCT7:
        fprintf(stderr, "Invalid funct7\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);
        printf("Funct7 = %x\n", i->funct7);
        break;
    case ERR_X0_RD:
        fprintf(stderr, "Invalid rd: Register x0 cannot be modified\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);            
        printf("Funct7 = %x\n", i->funct7);
        break;
    case ERR_ENV:
        fprintf(stderr, "Invalid environment instruction\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);            
        printf("Immediate = %x, ", i->immediate);
        break;
    default:
        break;
    }
}

// parse bits 0-6
void parseOpcode(instruction* i) {
    if (i == NULL) {
        return;
    }
    unsigned int mask = MASK_7BITS; 
    i->opcode = (enum Opcode)(i->input & mask); 

    switch (i->opcode) {
    case R_TYPE:
    case I_TYPE_LOAD:
    case I_TYPE_IMM:
    case S_TYPE:
    case B_TYPE:
    case ENV:
        break;
    case JALR:
        strcpy(i->instr, "jalr");
        break;
    case J_TYPE:
        strcpy(i->instr, "jal");
        break;
    case LUI:
        strcpy(i->instr, "lui");
        break;
    case AUIPC: 
        strcpy(i->instr, "auipc");
        break;
    default:
        i->opcode = INVALID;
        printDecodeError(i, ERR_OPCODE);
        break;
    }
}

// parse bits 12-14
void parseFunct3(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int mask = MASK_3BITS << BIT_FUNCT3; 
    unsigned int funct3 = (mask & i->input) >> BIT_FUNCT3;
    i->funct3 = funct3;
    switch(i->opcode) {
    case R_TYPE:
        switch(funct3) {
            case 0x0:
            case 0x1:
            case 0x2:
            case 0x3:
            case 0x4:
            case 0x5:
            case 0x6:
            case 0x7:
                i->funct3 = funct3;
                break;
            default:
                i->funct3 = funct3;
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case I_TYPE_IMM:
        switch(i->funct3) {
            case 0x0:
                strcpy(i->instr, "addi");
                break;
            case 0x1:
                strcpy(i->instr, "slli");
                break;
            case 0x2:
                strcpy(i->instr, "slti");
                break;
            case 0x3:
                strcpy(i->instr, "sltiu");
                break;
            case 0x4:
                strcpy(i->instr, "xori");
                break;
            case 0x5:
                break;
            case 0x6:
                strcpy(i->instr, "ori");
                break;
            case 0x7:
                strcpy(i->instr, "andi");
                break;
            default:
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case I_TYPE_LOAD:
        switch(funct3) {
            case 0x0:
                strcpy(i->instr, "lb");
                break;
            case 0x1:
                strcpy(i->instr, "lh");
                break;
            case 0x2:
                strcpy(i->instr, "lw");
                break;
            case 0x3:
                strcpy(i->instr, "ld");
                break;
            case 0x4:
                strcpy(i->instr, "lbu");
                break;
            case 0x5:
                strcpy(i->instr, "lhu");
                break;
            default:
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case S_TYPE:
        switch(funct3) {
        case 0x0:
            strcpy(i->instr, "sb");
            break;
        case 0x1:
            strcpy(i->instr, "sh");
            break;
        case 0x2:
            strcpy(i->instr, "sw");
            break;
        case 0x3:
            strcpy(i->instr, "sd");
            break; 
        default:
            printDecodeError(i, ERR_FUNCT3);
            return;
        }
        break;
    case B_TYPE:
        switch(i->funct3) {
            case 0x0:
                strcpy(i->instr, "beq");
                break;
            case 0x1:
                strcpy(i->instr, "bne");
                break;
            case 0x4:
                strcpy(i->instr, "blt");
                break;
            case 0x5:
                strcpy(i->instr, "bge");
                break;
            case 0x6:
                strcpy(i->instr, "bltu");
                break;
            case 0x7:
                strcpy(i->instr, "bgeu");
                break;
            default:
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case JALR:
    case ENV:
        if (i->funct3 != 0x0) {
            printDecodeError(i, ERR_FUNCT3);
            return;
        }
        break;
    case J_TYPE:
    case LUI:
    case AUIPC:
    default:
        break;
    }
}

// parse bits 25-32
void parseFunct7(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int mask = MASK_7BITS << BIT_FUNCT7; 
    unsigned int funct7 = (mask & i->input) >> BIT_FUNCT7;
    switch (i->opcode) {
    case R_TYPE:
        i->funct7 = funct7;
        switch (i->funct3) {
        case 0x0:
            switch (i->funct7) {
            case 0x0:
                strcpy(i->instr, "add");
                break;
            case 0x20:
                strcpy(i->instr, "sub");
                break;
            default:
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            break;
        case 0x1:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "sll");
            break;  
        case 0x2:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "slt");
            break;     
        case 0x3:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "sltu");
            break;                   
        case 0x4:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "xor");
            break; 
        case 0x5: 
            switch (i->funct7) {
            case 0x0:
                strcpy(i->instr, "srl");
                break;
            case 0x20:
                strcpy(i->instr, "sra");
                break;
            default:
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            break; 
        case 0x6:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "or");
            break; 
        case 0x7:
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_FUNCT7);
                return;
            }
            strcpy(i->instr, "and");       
            break;
        case 0x20:
            break;
        default:
            printDecodeError(i, ERR_FUNCT7);
            return;
        }

        break;
    case I_TYPE_IMM:
        switch (i->funct3) {
        case 0x1:
            i->funct7 = funct7;
            if (i->funct7 != 0x0) {
                printDecodeError(i, ERR_IMM);
                return;
            }
            break;
        case 0x5:
            i->funct7 = funct7;
            switch (i->funct7 = funct7) {
            case 0x0:
                strcpy(i->instr, "srli");
                break;
            case 0x20:
                strcpy(i->instr, "srai");
                break;
            default:
                printDecodeError(i, ERR_IMM);
                return;
            }
            break;
        default:
            i->immediate = (funct7 << IMM_UPPER);
            break;
        }
        break;
    case I_TYPE_LOAD:
    case S_TYPE:
    case JALR:
        i->immediate = (funct7 << IMM_UPPER);
        break;
    case B_TYPE:
        i->immediate = funct7;
        break;
    case ENV:
        i->immediate = funct7 << IMM_UPPER;
        if (i->immediate != 0x0) {
            printDecodeError(i, ERR_ENV);
        }
        break;
    case J_TYPE:
    case LUI:
    case AUIPC:
    default:
        break;
    }
}

// sign extend value v with the most significant bit sb
#define SIGNEX(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

// parse bits 7-11
void parseRd(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RD;
    unsigned int rd = (mask & i->input) >> BIT_RD;
    int temp;
    switch (i->opcode) {
    case R_TYPE:
    case I_TYPE_IMM:     
    case I_TYPE_LOAD:
    case J_TYPE:
    case JALR:
    case LUI:
    case AUIPC:
        i->rd = rd;
        if(rd == 0x0) {
            printDecodeError(i, ERR_X0_RD);
            return;
        } 
        break;
    case S_TYPE:
        temp = ((i->immediate) | rd);
        i->immediate = SIGNEX(temp, IMM12_MSB);
        break;
    case B_TYPE:
        temp = (((i->immediate >> 6) & MASK_1BIT) << 12) |
               (((rd >> 4) & MASK_1BIT) << 11) |
               ((i->immediate & MASK_6BITS) << 5) | 
               (((rd >> 1) & MASK_4BITS) << 1); 
        i->immediate = SIGNEX(temp, IMM13_MSB);
        break;
    case ENV:
        if (rd != 0x0) {
            printDecodeError(i, ERR_ENV);
        }
    default:
        break;
    }
}

// parse bits 15-19
void parseRs1(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RS1;
    unsigned int rs1 = (mask & i->input) >> BIT_RS1;
    switch (i->opcode) {
    case R_TYPE:
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
    case S_TYPE:
    case B_TYPE:
    case JALR:
        i->rs1 = rs1;
        break;
    case ENV:
        if (rs1 != 0x0) {
            printDecodeError(i, ERR_ENV);
        }
        break;
    case J_TYPE:
    case LUI:
    case AUIPC:
    default:
        break;
    }
}

// parse bits 20-24
void parseRs2(instruction* i)
{
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RS2;
    unsigned int rs2 = (mask & i->input) >> BIT_RS2;
    int signedCheck, temp;
    switch (i->opcode) {
    case R_TYPE:
    case S_TYPE:
    case B_TYPE:
        i->rs2 = rs2;
        break;
    case I_TYPE_IMM:
        switch (i->funct3) {
            case 0x1:
            case 0x5:
                i->immediate = rs2;
                break;
            default:
                signedCheck = ((i->immediate) | rs2);
                i->immediate = SIGNEX(signedCheck, IMM12_MSB);
                break;
        }
        break;
    case I_TYPE_LOAD:
    case JALR:
        signedCheck = ((i->immediate) | rs2);
        i->immediate = SIGNEX(signedCheck, IMM12_MSB);
        break;
    case J_TYPE:
        // imm[20|10:1|11|19:12] = risc format
        // imm[20|19:10|9|8:1] = parsed
        signedCheck = ((i->input >> 12) & MASK_20BITS) << 1;
        temp = (signedCheck & (MASK_1BIT << 20)) | // 20
               (((signedCheck >> 1) & MASK_8BITS) << 12) |  // 19:12
               (((signedCheck >> 9) & MASK_1BIT) << 11) |  // 11
               (((signedCheck >> 10) & MASK_10BITS) << 1); // 10:1
        i->immediate = SIGNEX(temp, IMM21_MSB);
        break;
    case LUI:
    case AUIPC:
        signedCheck = (i->input >> 12) & MASK_20BITS;
        i->immediate = SIGNEX(signedCheck, IMM20_MSB);
        break;
    case ENV:
        i->immediate = i->immediate | rs2;
        switch (i->immediate) {
        case 0x0:
            strcpy(i->instr, "ecall");   
            break;
        case 0x1:
            strcpy(i->instr, "ebreak");   
            break;
        default:
            printDecodeError(i, ERR_ENV);
            return;
        }
        break;
    default:
        break;
    }
}

void getAssemblyString(instruction* i) {
    if (i == NULL || i->opcode == INVALID) {
        return;
    }
    char tempString[32];

    switch (i->opcode) {
    case R_TYPE:
        snprintf(tempString, sizeof(tempString), "%s x%d, x%d, x%d", 
                                    i->instr, i->rd, i->rs1, i->rs2);
        break;
    case I_TYPE_IMM:
        snprintf(tempString, sizeof(tempString), "%s x%d, x%d, %d", 
                            i->instr, i->rd, i->rs1, i->immediate);
        break;
    case I_TYPE_LOAD:
    case JALR:
        snprintf(tempString, sizeof(tempString), "%s x%d, %d(x%d)", 
                            i->instr, i->rd, i->immediate, i->rs1);
        break;
    case S_TYPE:
        snprintf(tempString, sizeof(tempString), "%s x%d, %d(x%d)", 
                             i->instr, i->rs2, i->immediate, i->rs1);
        break;
    case B_TYPE:
        snprintf(tempString, sizeof(tempString), "%s x%d, x%d, %d",
                           i->instr, i->rs1, i->rs2, i->immediate);
        break;
    case J_TYPE:
    case LUI:
    case AUIPC:
        snprintf(tempString, sizeof(tempString), "%s x%d, %d",
                               i->instr, i->rd, i->immediate);     
        break;
    case ENV:
        snprintf(tempString, sizeof(tempString), "%s", i->instr);     
        break;  
    default:
        return;
    }
    i->assemblyStr = (char*) malloc(strlen(tempString) + 1);
    if (i->assemblyStr == NULL) {
        return;
    }
    strcpy(i->assemblyStr, tempString);
}

void printdecodedAssembly(instruction* i) {
    if (i == NULL || i->opcode == INVALID || i->assemblyStr == NULL) {
        return;
    }
    printf("%s\n", i->assemblyStr);
}

instruction* createDecodedInstruction(unsigned int hex) {
    instruction* i = malloc(sizeof(instruction));
    i->input = hex;
    parseOpcode(i); 
    parseFunct3(i);
    parseFunct7(i);
    parseRd(i);
    parseRs1(i);
    parseRs2(i);
    getAssemblyString(i);
    if (i->opcode == INVALID) {
        return NULL;
    }
    return i;
}

bool decodeInstruction(char* input) {
    unsigned int n = 0;
    if (convertStrToUint(input, &n)) {
        instruction* i = createDecodedInstruction(n);
        if (i == NULL) {
            return false;
        }
        printdecodedAssembly(i);
        free(i->assemblyStr);
        free(i);
        return true;
    }
    return false;
}
