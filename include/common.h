#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum Format { 
    INVALID,
    R, 
    I, 
    S,
    B 
};

enum Opcode {
    R_TYPE = 0x33u, 
    I_TYPE_IMM = 0x13u,
    I_TYPE_LOAD = 0x3u,
    S_TYPE = 0x23u,
    B_TYPE = 0x63u
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
    MASK_1BIT = 0x1,
    MASK_3BITS = 0x7, 
    MASK_4BITS = 0xf,
    MASK_5BITS = 0x1f, 
    MASK_6BITS = 0x3f,
    MASK_7BITS = 0x7f 
}; 

enum Imm {
    IMM_LOWER, 
    IMM_UPPER = 5, 
    IMM12_MSB = 11,
    IMM13_MSB = 12
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
    char instr[6]; 
    char* assemblyStr;
} instruction;

#endif 