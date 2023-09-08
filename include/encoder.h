#ifndef ENCODER_H_
#define ENCODER_H_

#include "../include/common.h"
#include "../include/decoder.h"

// add x1, x2, x3
// sub x1, x2, x3
// addi x9, x18, 2047
// addi x31, x6, -2048
// ld x9, -1 (x18) 
// sd x1, 1000 (x2)

// ADD, SUB
// instr rd, rs1, rs2

// ADDI
// instr rd, rs1, imm

// LD 
// instr rd, imm (rs1)

// SD
// instr rs2, imm (rs1)

// check instruction 
// input
// if valid 0,5?
// instr, type, 
bool isValidInstruction(const char* s);

// instr
// obtain opcode
// bits 0-6 -> input

// obtain funct3
// instr
// bits 12-14 -> input

// obtain funct7
// input
// funct7 || imm (up)
// bits 25-31 ->

// obtain rd
// bits 7-11 ->

// obtain rs1
// bits 15-19 ->

// obtain rs2
// bits 20-24 ->

// obtain input

#endif