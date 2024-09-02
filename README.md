# RISCV-Assembler

## Project Description
The RISC-V Assembler is a C-based program that translates RISC-V assembly language code into machine code. This project serves as a foundational tool for understanding the assembly language and its corresponding machine representation. It aims to provide users with a practical understanding of the translation process, enabling them to work with low-level programming efficiently.

## What the Application Does
The assembler takes RISC-V assembly instructions as input and converts them into machine code format. It supports a range of RISC-V instructions, including:
- R-type instructions (add, sub, and, or, xor, sll, srl, sra)
- I-type instructions (addi, andi, ori, xori, slli, srli, srai, ld, lw, lh, lb, lwu, lhu, lbu)
- S-type instructions (sd, sw, sh, sb, jalr)
- B-type instructions (beq, bne, blt, bge, bltu, bgeu)
- U-type instructions(lui)
- J-type instructions(jal)
By generating machine code, the assembler allows users to see how assembly instructions are represented in binary, facilitating a deeper understanding of the RISC-V architecture.

## Technologies Used
Programming Language: C
Data Structures: Arrays for managing registers, strings for instruction parsing, and bitwise operations for machine code generation
Input Handling: Standard input for reading assembly instructions
These technologies were chosen for their efficiency in handling low-level operations, which is crucial for an assembler.

## Challenges faced:
During the development of the assembler, several challenges were encountered, including:
Instruction Parsing: Designing a parser that accurately interprets assembly instructions and distinguishes between different types and formats.
Error Handling: Implementing robust error checking to manage invalid instructions, such as incorrect operands or unsupported instructions, ensuring the assembler provides meaningful feedback to users.
Machine Code Generation: Accurately translating each instruction into its binary representation while considering the specific encoding rules of the RISC-V architecture.


## Future Features
There are several enhancements planned for the assembler, including:
Support translation of pseudo instructions(e.g., j, mv)
User Interface could be built

## Contents
demo.c : The main assembler code responsible for translating assembly instructions into machine code.
input.s: A sample file containing RISC-V assembly instructions to be processed by the assembler.
output.hex: The output file that contains the translated machine code in hexadecimal format.

