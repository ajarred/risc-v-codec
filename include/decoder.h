#ifndef DECODER_H_
#define DECODER_H_

#include "../include/common.h"

bool isHex(char* str);

bool convertStrToUint(char* str, unsigned int *n);

void printInstructions(instruction* i);

instruction* createInstruction(unsigned int hex);

bool decodeInstruction(char* input);

#endif