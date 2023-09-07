#ifndef COMMON_H_
#define COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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