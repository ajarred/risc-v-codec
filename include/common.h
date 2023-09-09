#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

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

typedef struct instruction {
    unsigned int input;
    unsigned int type;
    unsigned int opcode;
    unsigned int rd;
    unsigned int funct3;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int funct7;
    int immediate;
    char instr[4]; 
} instruction;

#endif 