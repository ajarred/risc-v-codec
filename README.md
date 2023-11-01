# risc-v-codec
This is a CLI implementation of a RISC-V Instruction Codec (Decoder/Encoder). The program can read hex numbers and outputs the corresponding assembly instruction, and vice versa.

## Usage
Clone the repository or download the source code.
Open a terminal and navigate to the project directory.

## Compilation
To compile the source code, run the following command:
```bash
make
```

## Running
After compilation, run the program with the following command:
```bash
./risc
```
## Current Instruction List
- R-Type: add, sub, xor, and, sll, srl, sra, slt, sltu
- I-Type: addi, ld
- S-Type: sd
- B-Type: beq
- J-Type: jal
- U-Type: lui, auipc

