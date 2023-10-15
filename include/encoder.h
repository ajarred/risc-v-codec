#ifndef ENCODER_H_
#define ENCODER_H_

#include "../include/common.h"

bool isValidInstruction(const char* s);

void obtainInput(instruction* i);

bool encodeInstruction(const char* s);

#endif