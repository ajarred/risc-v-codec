#ifndef DECODER_H_
#define DECODER_H_

#include "../include/common.h"

bool isHex(const char* str);

bool convertStrToUint(const char* str, unsigned int *n);

void printError(instruction* i, const enum ErrorType err);

void parseOpcode(instruction* i);

#endif