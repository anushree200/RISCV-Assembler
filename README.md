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
- Programming Language: C
- Data Structures: Arrays for managing registers, strings for instruction parsing, and bitwise operations for machine code generation
- Input Handling: Standard input for reading assembly instructions
These technologies were chosen for their efficiency in handling low-level operations, which is crucial for an assembler.

## Challenges faced:
During the development of the assembler, several challenges were encountered, including:
- Instruction Parsing: Designing a parser that accurately interprets assembly instructions and distinguishes between different types and formats.
- Error Handling: Implementing robust error checking to manage invalid instructions, such as incorrect operands or unsupported instructions, ensuring the assembler provides meaningful feedback to users.
- Machine Code Generation: Accurately translating each instruction into its binary representation while considering the specific encoding rules of the RISC-V architecture.


## Future Features
There are several enhancements planned for the assembler, including:
- Support translation of pseudo instructions(e.g., j, mv)
- User Interface could be built

## Contents
- demo2.c : The main assembler code responsible for translating assembly instructions into machine code.
- input.s: A sample file containing RISC-V assembly instructions to be processed by the assembler.
- output.hex: The output file that contains the translated machine code in hexadecimal format.
To know the details of each function, variables and the flow of code execution, refer the report.pdf

## Prerequisites
- Instruction Set Architecture (ISA): Familiarity with RISC-V or the specific ISA you're working with.
- Assembly Language: Knowledge of writing and understanding assembly language instructions.
- Binary and Hexadecimal Notation: Ability to work with and convert between binary, hexadecimal, and decimal systems.
- Basic C Programming: Proficiency in C programming for implementing the assembler logic.
- Instruction Formats: Understanding the various instruction formats and their encodings, such as:
            - R-type: Register-based instructions
            - I-type: Immediate value instructions
            - S-type: Store instructions
            - B-type: Branch instructions
            - J-type: Jump instructions
            - U-type: Upper Immediate instructions

### R-Format

<table>
  <tr>
    <th>31-25</th>
    <th>24-20</th>
    <th>19-15</th>
    <th>14-12</th>
    <th>11-7</th>
    <th>6-0</th>
  </tr>
  <tr>
     <td>`funct7`</td>
     <td>`rs2`</td>
     <td>`rs1`</td>
     <td>`funct3`</td>
     <td>`rd</td>
     <td>`opcode`</td>
  </tr>
</table>

- funct7: A 7-bit function code that helps specify the exact operation (e.g., add, sub).
- rs2: A 5-bit field representing the second source register.
- rs1: A 5-bit field representing the first source register.
- funct3: A 3-bit function code that further specifies the operation, often used in combination with funct7.
- rd: A 5-bit field representing the destination register where the result of the operation is stored.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, R format: 0110011

### I-Format

<table>
  <tr>
    <th>31-20</th>
    <th>19-15</th>
    <th>14-12</th>
    <th>11-7</th>
    <th>6-0</th>
  </tr>
  <tr>
     <td>`Immediate[11:0]`</td>
     <td>`rs1`</td>
     <td>`funct3`</td>
     <td>`rd</td>
     <td>`opcode`</td>
  </tr>
</table>

- imm[11:0]: A 12-bit immediate value that is sign-extended to the register width.
- rs1: A 5-bit field representing the first source register.
- funct3: A 3-bit function code that specifies the operation.
- rd: A 5-bit field representing the destination register.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, I format: 0010011 , 1100111[Only for jalr]

### S-Format
