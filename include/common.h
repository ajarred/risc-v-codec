#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

enum Opcode {
    INVALID,
    R_TYPE = 0x33u, 
    I_TYPE_IMM = 0x13u,
    I_TYPE_LOAD = 0x3u,
    S_TYPE = 0x23u,
    B_TYPE = 0x63u,
    J_TYPE = 0x6fu,
    JALR = 0x67u,
    LUI = 0x37u,
    AUIPC = 0x17u,
    ENV = 0x73u
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
    MASK_7BITS = 0x7f,
    MASK_8BITS = 0xffu,
    MASK_10BITS = 0x3ffu,
    MASK_20BITS = 0xfffffu
}; 

enum Imm {
    IMM_LOWER, 
    IMM_UPPER = 5, 
    IMM12_MSB = 11,
    IMM13_MSB = 12,
    IMM20_MSB = 19,
    IMM21_MSB = 20,
};

typedef struct instruction {
    unsigned int input;
    unsigned int opcode;
    unsigned int rd;
    unsigned int funct3;
    unsigned int rs1;
    unsigned int rs2;
    unsigned int funct7;
    int immediate;
    char instr[7]; 
    char* assemblyStr;
} instruction;

#endif 