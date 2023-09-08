#include "../include/decoder.h"

enum Format { 
    INVALID,
    R, 
    I, 
    S 
};

enum ErrorType {
    ERR_OPCODE,
    ERR_FUNCT3,
    ERR_FUNCT7,
    ERR_X0_RD,
    ERR_X0_RS1,
    ERR_IMM,
};

enum Opcode {
    ADD = 0x33u, 
    ADDI = 0x13u,
    LD = 0x3u,
    SD = 0x23u
};

enum BitPlace {
    BIT_OPCODE = 0,
    BIT_RD = 7,
    BIT_FUNCT3 = 12,
    BIT_RS1 = 15,
    BIT_RS2 = 20,
    BIT_FUNCT7 = 25,
};

enum MaskValues {
    MASK_3BITS = 0x7, 
    MASK_5BITS = 0x1f, 
    MASK_7BITS = 0x7f 
};

enum Imm {
    IMM_LOWER, 
    IMM_UPPER = 5, 
    SIGN_EXTENDED_SHIFT = 20 
};

bool isHex(const char* str) {
    size_t length = strlen(str);
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

bool convertStrToUint(const char* str, unsigned int *n) {
    if(!isHex(str))
    {
        fprintf(stderr, "Invalid input. Please input a valid hex string\n");
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
        printf("Funct7 = %x, ", i->funct7);
        break;
    case ERR_X0_RS1:
        i->type = INVALID;
        fprintf(stderr, "Invalid rs1: Register x0 cannot be modified\n");
        printf("Opcode = %x, ", i->opcode);
        printf("Funct3 = %x, ", i->funct3);            
        printf("Funct7 = %x, ", i->funct7);
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