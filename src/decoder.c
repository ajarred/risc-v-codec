#include "../include/decoder.h"

enum ErrorType {
    ERR_OPCODE,
    ERR_FUNCT3,
    ERR_FUNCT7,
    ERR_X0_RD,
    ERR_IMM,
};

enum Opcode {
    ADD = 0x33u, 
    ADDI = 0x13u,
    LD = 0x3u,
    SD = 0x23u
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
    // printf("%x\n", *n);
    return true;
}

void printError(instruction* i, const enum ErrorType err) {
    switch (err) {
    case ERR_OPCODE:
        i->type = INVALID;
        fprintf(stderr, "Opcode not found\n");
        printf("Opcode = %x\n", i->opcode);
        break;
    case ERR_FUNCT3:
        i->type = INVALID;
        switch(i->opcode) {
        case ADD:
        case ADDI:
            fprintf(stderr, "Invalid funct3. Funct3 must be 0x0\n");
            printf("Opcode = %x, ", i->opcode);
            printf("Funct3 = %x\n", i->funct3);
            break;
        case LD:
        case SD:
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
        case ADD:
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
    case ERR_IMM:
        i->type = INVALID;
        fprintf(stderr, "Immediate value is out of range\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);            
        printf("Funct7 = %x, ", i->funct7);
        printf("Immediate = %x\n", i->immediate);
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
    i->opcode = (enum Opcode)(i->input & mask); // convert uint to enum type
    // printf("%x\n", i->opcode);

    switch (i->opcode) {
    case ADD:
        i->type = R;
        break;
    case LD:
    case ADDI:
        i->type = I;
        break;
    case SD:
        i->type = S;
        break;
    default:
        i->type = INVALID;
        printError(i, ERR_OPCODE);
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
    case ADD:
        if (i->funct3 != 0x0) {
            printError(i, ERR_FUNCT3);
            return;
        }
        // printf("funct3 = %x\n", i->funct3);
        break;
    case ADDI:
        if (i->funct3 != 0x0) {
            printError(i, ERR_FUNCT3);
            return;
        }
        // printf("funct3 = %x\n", i->funct3);
        strcpy(i->instr, "addi");
        // printf("instr = %s\n", i->instr);
        break;
    case LD:
        if (i->funct3 != 0x3) {
            printError(i, ERR_FUNCT3);
            return;
        }
        // printf("funct3 = %x\n", i->funct3);
        strcpy(i->instr, "ld");
        // printf("instr = %s\n", i->instr);
        break;
    case SD:
        if (i->funct3 != 0x3) {
            printError(i, ERR_FUNCT3);
            return;
        }
        // printf("funct3 = %x\n", i->funct3);
        strcpy(i->instr, "sd");
        // printf("instr = %s\n", i->instr);
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
    case ADD:
        i->funct7 = funct7;
        if (i->funct7 != 0x0 && funct7 != 0x20)
        {
            printError(i, ERR_FUNCT7);
            return;
        }
        // printf("funct7 = %x\n", i->funct7);
        switch (i->funct7) {
        case 0x0:
            strcpy(i->instr, "add");
            // printf("instr = %s\n", i->instr);
            break;
        case 0x20:
            strcpy(i->instr, "sub");
            // printf("instr = %s\n", i->instr);
            break;
        default:
            break;
        }
        break;
    case ADDI:
    case LD:
    case SD:
        i->immediate = (funct7 << IMM_UPPER);
        // printf("immediate (upper) = %d\n", i->immediate);
        break;
    default:
        break;
    }
}

// parse bits 7-11
void parseRd(instruction* i) {
    if (i->type == INVALID) {
        return;
    }
    unsigned int mask = MASK_5BITS << BIT_RD;
    unsigned int rd = (mask & i->input) >> BIT_RD;
    int signedCheck;
    switch (i->opcode) {
    case ADD:
    case ADDI:     
    case LD:
        i->rd = rd;
        if(rd == 0x0) {
            printError(i, ERR_X0_RD);
            return;
        } 
        // printf("rd = %x\n", rd);
        break;
    case SD:
        // printf("immediate (lower) = %d\n", rd);
        // printf("immediate (upper) = %d\n", i->immediate);
        signedCheck = ((i->immediate) | rd) << SIGN_EXTENDED_SHIFT;
        signedCheck >>= SIGN_EXTENDED_SHIFT;  
        i->immediate = signedCheck;
        if (i->immediate < MIN_SIGNED_BIT || i->immediate > MAX_SIGNED_BIT) {
            printError(i, ERR_IMM);
            return;
        }
        // printf("immediate (total) = %d\n", i->immediate);
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
    case ADD:
    case ADDI:
    case LD:
    case SD:
        // printf("rs1 = %d\n", i->rs1);
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
    case ADD:
    case SD:
        i->rs2 = rs2;
        // printf("rs2 = %d\n", i->rs2);
        break;
    case ADDI:
    case LD:
        // printf("immediate (lower) = %d\n", rs2);
        // printf("immediate (upper) = %d\n", i->immediate);
        signedCheck = ((i->immediate) | rs2) << SIGN_EXTENDED_SHIFT;
        signedCheck >>= SIGN_EXTENDED_SHIFT; 
        i->immediate = signedCheck;
        if (signedCheck < MIN_SIGNED_BIT || signedCheck > MAX_SIGNED_BIT) {
                printError(i, ERR_IMM);
                return;
        }
        // printf("immediate (total) = %d\n", i->immediate);
        break;
    default:
        break;
    }
}

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
        case ADDI:
            printf("%s x%d, x%d, %d\n", 
            i->instr, i->rd, i->rs1, i->immediate);
            break;
        case LD:
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

void decodeInstruction(instruction* i) {
    parseOpcode(i); 
    parseFunct3(i);
    parseFunct7(i);
    parseRd(i);
    parseRs1(i);
    parseRs2(i);
    printInstructions(i);
}