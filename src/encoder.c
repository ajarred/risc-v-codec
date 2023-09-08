#include "../include/encoder.h"

bool isValidInstruction(const char* s) {
    if (strncmp(s, "add", 3) == 0 ||
        strncmp(s, "sub", 3) == 0 ||
        strncmp(s, "addi",4) == 0 ||
        strncmp(s, "ld",  4) == 0 || 
        strncmp(s, "sd",  4) == 0) {
        return true;
    }
    return false;
}