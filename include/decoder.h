#ifndef DECODER_H_
#define DECODER_H_

#include "../include/common.h"

bool isHex(char* str);

bool convertStrToUint(char* str, unsigned int *n);

void parseOpcode(instruction* i);

void parseFunct3(instruction* i);

void parseFunct7(instruction* i);

void parseRd(instruction* i);

void parseRs1(instruction* i);

void parseRs2(instruction* i);

void printInstructions(instruction* i);

void decodeInstruction(instruction* i);

#endif