#ifndef DECODER_H_
#define DECODER_H_

#include "../include/common.h"

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

bool isHex(const char* str);

bool convertStrToUint(const char* str, unsigned int *n);

void printError(instruction* i, const enum ErrorType err);

void parseOpcode(instruction* i);

void parseFunct3(instruction* i);

#endif