#ifndef DECODER_H_
#define DECODER_H_

#include "../include/common.h"

// 12-bit immediate value limits
#define MIN_SIGNED_BIT -2048
#define MAX_SIGNED_BIT 2047

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

bool isHex(char* str);

bool convertStrToUint(char* str, unsigned int *n);

void printError(instruction* i, const enum ErrorType err);

void parseOpcode(instruction* i);

void parseFunct3(instruction* i);

void parseFunct7(instruction* i);

void parseRd(instruction* i);

void parseRs1(instruction* i);

void parseRs2(instruction* i);

void printInstructions(instruction* i);

void decodeInstruction(instruction* i);

#endif