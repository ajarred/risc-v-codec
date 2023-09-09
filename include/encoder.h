#ifndef ENCODER_H_
#define ENCODER_H_

#include "../include/common.h"

bool isValidInstruction(const char* s);

bool obtainInstruction(instruction* i, const char* s);

void obtainArguments(instruction* i, const char* s);

void obtainFunct7(instruction*i);

void obtainFunct3(instruction* i);

void obtainOpcode(instruction* i);

void obtainInput(instruction* i);

#endif