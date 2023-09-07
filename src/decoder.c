#include "../include/decoder.h"

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
    ADD = 0x33, 
    ADDI = 0x13,
    LD = 0x3,
    SD = 0x23
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

int isHex(const char* str) {
    size_t length = strlen(str);
    if (length <= 10 && strncmp(str, "0x", 2) == 0) {
        for (size_t i = 2; i < length; i++) {
            if (!isxdigit((unsigned char)str[i])) {
                return 0;
            }
        }
        return 1;
    }
    return 0;
}

