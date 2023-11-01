#ifndef ENCODER_H_
#define ENCODER_H_

#include "../include/common.h"

bool isValidInstruction(const char* str);

void obtainInput(instruction* i);

instruction* createEncodedInstruction(const char* s);

bool encodeInstruction(const char* s);

#endif