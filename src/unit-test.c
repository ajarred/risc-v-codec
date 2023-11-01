#include "../include/decoder.h"
#include "../include/encoder.h"
#include "../include/unit-test.h"

void runUnitTest() {
    printf("Running tests...\n\n");

    // add x0, x1, x2
    instruction* test = createDecodedInstruction(0x00208033);
    assert(test == NULL);
    free(test);

    // addi x0, x1, 1
    test = createDecodedInstruction(0x00108013);
    assert(test == NULL);
    free(test);

    test = createDecodedInstruction(0x3100B3);
    assert(strcmp(test->assemblyStr, "add x1, x2, x3") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("add x1, x2, x3");
    assert(test->input == 0x3100B3);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sub x29, x31, x30");
    assert(test->input == 0x41ef8eb3);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x41ef8eb3);
    assert(strcmp(test->assemblyStr, "sub x29, x31, x30") == 0);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x800a0513);
    assert(strcmp(test->assemblyStr, "addi x10, x20, -2048") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("addi x10, x20, -2048");
    assert(test->input == 0x800a0513);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x7ffa0513);
    assert(strcmp(test->assemblyStr, "addi x10, x20, 2047") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("addi x10, x20, 2047");
    assert(test->input == 0x7ffa0513);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x80013083);
    assert(strcmp(test->assemblyStr, "ld x1, -2048(x2)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("ld x1, -2048(x2)");
    assert(test->input == 0x80013083);
    free(test->assemblyStr);
    free(test);   

    test = createDecodedInstruction(0x7ffa3983);
    assert(strcmp(test->assemblyStr, "ld x19, 2047(x20)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("ld x19, 2047(x20)");
    assert(test->input == 0x7ffa3983);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x801fb0a3);
    assert(strcmp(test->assemblyStr, "sd x1, -2047(x31)") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sd x1, -2047(x31)");
    assert(test->input == 0x801fb0a3);
    free(test->assemblyStr);
    free(test);     

    test = createDecodedInstruction(0x80000063);
    assert(strcmp(test->assemblyStr, "beq x0, x0, -4096") == 0);
    free(test->assemblyStr);
    free(test); 

    test = createDecodedInstruction(0x7fef8fe3);
    assert(strcmp(test->assemblyStr, "beq x31, x30, 4094") == 0);
    free(test->assemblyStr);
    free(test); 

    test = createEncodedInstruction("beq x0, x0, -4096");
    assert(test->input == 0x80000063);
    free(test->assemblyStr);
    free(test); 

    test = createEncodedInstruction("beq x31, x30, 4094");
    assert(test->input == 0x7fef8fe3);
    free(test->assemblyStr);
    free(test);  

    test = createDecodedInstruction(0x01e7c0b3);
    assert(strcmp(test->assemblyStr, "xor x1, x15, x30") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("xor x1, x15, x30");
    assert(test->input == 0x01e7c0b3);
    free(test->assemblyStr);
    free(test);  

    test = createDecodedInstruction(0x01e7e0b3);
    assert(strcmp(test->assemblyStr, "or x1, x15, x30") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("or x1, x15, x30");
    assert(test->input == 0x01e7e0b3);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x01e7f0b3);
    assert(strcmp(test->assemblyStr, "and x1, x15, x30") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("and x1, x15, x30");
    assert(test->input == 0x01e7f0b3);
    free(test->assemblyStr);
    free(test);  

    test = createDecodedInstruction(0x01ff1eb3);
    assert(strcmp(test->assemblyStr, "sll x29, x30, x31") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sll x29, x30, x31");
    assert(test->input == 0x01ff1eb3);
    free(test->assemblyStr);
    free(test);  

    test = createDecodedInstruction(0x01ff5eb3);
    assert(strcmp(test->assemblyStr, "srl x29, x30, x31") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("srl x29, x30, x31");
    assert(test->input == 0x01ff5eb3);
    free(test->assemblyStr);
    free(test); 

    test = createDecodedInstruction(0x40ff50b3);
    assert(strcmp(test->assemblyStr, "sra x1, x30, x15") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sra x1, x30, x15");
    assert(test->input == 0x40ff50b3);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x0138a1b3);
    assert(strcmp(test->assemblyStr, "slt x3, x17, x19") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("slt x3, x17, x19");
    assert(test->input == 0x0138a1b3);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x00a43333);
    assert(strcmp(test->assemblyStr, "sltu x6, x8, x10") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("sltu x6, x8, x10");
    assert(test->input == 0x00a43333);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x800000ef);
    assert(strcmp(test->assemblyStr, "jal x1, -1048576") == 0);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x7ffff16f);
    assert(strcmp(test->assemblyStr, "jal x2, 1048574") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("jal x1, -1048576");
    assert(test->input == 0x800000ef);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("jal x2, 1048574");
    assert(test->input == 0x7ffff16f);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x80000217);
    assert(strcmp(test->assemblyStr, "auipc x4, -524288") == 0);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x7ffff217);
    assert(strcmp(test->assemblyStr, "auipc x4, 524287") == 0);
    free(test->assemblyStr);
    free(test);  

    test = createDecodedInstruction(0x7ffff637);
    assert(strcmp(test->assemblyStr, "lui x12, 524287") == 0);
    free(test->assemblyStr);
    free(test);

    test = createDecodedInstruction(0x80000637);
    assert(strcmp(test->assemblyStr, "lui x12, -524288") == 0);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("lui x12, -524288");
    assert(test->input == 0x80000637);
    free(test->assemblyStr);
    free(test);

    test = createEncodedInstruction("auipc x4, -524288");
    assert(test->input == 0x80000217);
    free(test->assemblyStr);
    free(test);

    printf("\nPassed all tests\n");
}