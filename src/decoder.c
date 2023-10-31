#include "../include/decoder.h"

enum ErrorType {
    ERR_OPCODE,
    ERR_FUNCT3,
    ERR_FUNCT7,
    ERR_X0_RD,
};

enum Opcode {
    R_TYPE = 0x33u, 
    I_TYPE_IMM = 0x13u,
    I_TYPE_LOAD = 0x3u,
    S_TYPE = 0x23u,
    B_TYPE = 0x63u
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
    switch (err) {
    case ERR_OPCODE:
        i->type = INVALID;
        fprintf(stderr, "Opcode not found\n");
        printf("Opcode = %x\n", i->opcode);
        break;
    case ERR_FUNCT3:
        i->type = INVALID;
        switch(i->opcode) {
        case R_TYPE:
        case I_TYPE_IMM:
            fprintf(stderr, "Invalid funct3. Funct3 must be 0x0\n");
            printf("Opcode = %x, ", i->opcode);
            printf("Funct3 = %x\n", i->funct3);
            break;
        case I_TYPE_LOAD:
        case S_TYPE:
            fprintf(stderr, "Invalid funct3. Funct3 must be 0x3\n");
            printf("Opcode = %x, ", i->opcode);
            printf("Funct3 = %x\n", i->funct3);
            break;
        default:
            break;
        }
        break;
    case ERR_FUNCT7:
        switch(i->opcode) {
        case R_TYPE:
            i->type = INVALID;
            fprintf(stderr, "Invalid funct7. Funct7 must be either 0x0 or 0x20\n");
            printf("Opcode = %x, ", i->opcode);
            printf("Funct3 = %x, ", i->funct3);
            printf("Funct7 = %x\n", i->funct7);
            break;
        default:
            break;
        }
        break;
    case ERR_X0_RD:
        i->type = INVALID;
        fprintf(stderr, "Invalid rd: Register x0 cannot be modified\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);            
        printf("Funct7 = %x\n", i->funct7);
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
        i->type = R;
        break;
    case I_TYPE_LOAD:
    case I_TYPE_IMM:
        i->type = I;
        break;
    case S_TYPE:
        i->type = S;
        break;
    case B_TYPE:
        i->type = B;
        break;
    default:
        i->type = INVALID;
        printDecodeError(i, ERR_OPCODE);
        break;
    }
}

// parse bits 12-14
void parseFunct3(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_3BITS << BIT_FUNCT3; 
    i->funct3 = (mask & i->input) >> BIT_FUNCT3;
    switch(i->opcode) {
    case R_TYPE:
        if (i->funct3 != 0x0) {
            printDecodeError(i, ERR_FUNCT3);
            return;
        }
        break;
    case I_TYPE_IMM:
        switch(i->funct3) {
            case 0x0:
                strcpy(i->instr, "addi");
                break;
            default:
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case I_TYPE_LOAD:
        switch(i->funct3) {
            case 0x3:
                strcpy(i->instr, "ld");
                break;
            default:
                printDecodeError(i, ERR_FUNCT3);
                return;
        }
        break;
    case S_TYPE:
        switch(i->funct3) {
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
            default:
                printDecodeError(i, ERR_FUNCT3);
                break;
        }
        break;
    default:
        break;
    }
}

// parse bits 25-32
void parseFunct7(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_7BITS << BIT_FUNCT7; 
    unsigned int funct7 = (mask & i->input) >> BIT_FUNCT7;
    switch (i->opcode) {
    case R_TYPE:
        i->funct7 = funct7;
        if (i->funct7 != 0x0 && funct7 != 0x20)
        {
            printDecodeError(i, ERR_FUNCT7);
            return;
        }
        switch (i->funct7) {
        case 0x0:
            strcpy(i->instr, "add");
            break;
        case 0x20:
            strcpy(i->instr, "sub");
            break;
        default:
            break;
        }
        break;
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
    case S_TYPE:
        i->immediate = (funct7 << IMM_UPPER);
        break;
    case B_TYPE:
        i->immediate = funct7;
        break;
    default:
        break;
    }
}

// sign extend value v with the most significant bit sb
#define SIGNEX(v, sb) ((v) | (((v) & (1 << (sb))) ? ~((1 << (sb))-1) : 0))

// parse bits 7-11
void parseRd(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RD;
    unsigned int rd = (mask & i->input) >> BIT_RD;
    int temp;
    switch (i->opcode) {
    case R_TYPE:
    case I_TYPE_IMM:     
    case I_TYPE_LOAD:
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
    default:
        break;
    }
}

// parse bits 15-19
void parseRs1(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RS1;
    i->rs1 = (mask & i->input) >> BIT_RS1;
    switch (i->opcode) {
    case R_TYPE:
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
    case S_TYPE:
    case B_TYPE:
        break;
    default:
        break;
    }
}

// parse bits 20-24
void parseRs2(instruction* i)
{
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RS2;
    unsigned int rs2 = (mask & i->input) >> BIT_RS2;
    int signedCheck;
    switch (i->opcode) {
    case R_TYPE:
    case S_TYPE:
    case B_TYPE:
        i->rs2 = rs2;
        break;
    case I_TYPE_IMM:
    case I_TYPE_LOAD:
        signedCheck = ((i->immediate) | rs2);
        i->immediate = SIGNEX(signedCheck, IMM12_MSB);
        break;
    default:
        break;
    }
}

/*
void printInstructions(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    switch (i->type) {
    case R:
        printf("%s x%d, x%d, x%d\n", 
        i->instr, i->rd, i->rs1, i->rs2);
        break;
    case I:
        switch (i->opcode) {
        case I_TYPE_IMM:
            printf("%s x%d, x%d, %d\n", 
            i->instr, i->rd, i->rs1, i->immediate);
            break;
        case I_TYPE_LOAD:
            printf("%s x%d, %d (x%d)\n", 
            i->instr, i->rd, i->immediate, i->rs1);
            break;
        default:
            break;
        }
        break;
    case S:
        printf("%s x%d, %d (x%d)\n", 
        i->instr, i->rs2, i->immediate, i->rs1);
        break;
    default:
        break;
    }
}
*/

void getAssemblyString(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    char tempString[32];

    switch (i->type) {
    case R:
        snprintf(tempString, sizeof(tempString), "%s x%d, x%d, x%d", 
                                    i->instr, i->rd, i->rs1, i->rs2);
        break;
    case I:
        switch (i->opcode) {
        case I_TYPE_IMM:
            snprintf(tempString, sizeof(tempString), "%s x%d, x%d, %d", 
                                 i->instr, i->rd, i->rs1, i->immediate);
            break;
        case I_TYPE_LOAD:
            snprintf(tempString, sizeof(tempString), "%s x%d, %d(x%d)", 
                                  i->instr, i->rd, i->immediate, i->rs1);
            break;
        default:
            break;
        }
        break;
    case S:
        snprintf(tempString, sizeof(tempString), "%s x%d, %d(x%d)", 
                             i->instr, i->rs2, i->immediate, i->rs1);
        break;
    case B:
        snprintf(tempString, sizeof(tempString), "%s x%d, x%d, %d",
                           i->instr, i->rs1, i->rs2, i->immediate);
        break;
    default:
        break;
    }
    i->assemblyStr = (char*)malloc(strlen(tempString) + 1);
    if (i->assemblyStr == NULL) {
        return;
    }
    strcpy(i->assemblyStr, tempString);
}

void printdecodedAssembly(instruction* i) {
    if (i->assemblyStr == NULL) {
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
    if (i->type == INVALID) {
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
