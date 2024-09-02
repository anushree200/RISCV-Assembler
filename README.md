# RISCV-Assembler

## CONTENTS 
This file has been divided into the following sections for easy reference:
- Project Description
- Hardware requirements to run this program.
- Operating System and its requirements.
- Installation of the C compiler
- Input file
- About the program
- Executing the program.


## Project Description
The RISC-V Assembler Project is a C-based program that translates RISC-V assembly language code into machine code. This project serves as a foundational tool for understanding the assembly language and its corresponding machine representation. It aims to provide users with a practical understanding of the translation process, enabling them to work with low-level programming efficiently.

## What the Application Does
The assembler takes RISC-V assembly instructions as input and converts them into machine code format. It supports a range of RISC-V instructions, including:
- R-type instructions (add, sub, and, or, xor, sll, srl, sra)
- I-type instructions (addi, andi, ori, xori, slli, srli, srai, ld, lw, lh, lb, lwu, lhu, lbu)
- S-type instructions (sd, sw, sh, sb, jalr)
- B-type instructions (beq, bne, blt, bge, bltu, bgeu)
- U-type instructions(lui)
- J-type instructions(jal)

By generating machine code, the assembler allows users to see how assembly instructions are represented in binary, facilitating a deeper understanding of the RISC-V architecture.

## Hardware requirements to compile and run this program
- A good processor with a minimum quantity ram and a minimum harddisk space is enough to run this program. 
Actually this program will not consume much of memory and the harddisk space.
- Monitor to see the output
- Keyboard to compile and execute the code is required

## Operating System and its requirements
This code can run in any operating system as long as that operating system has a C compiler installed. 
The gcc compiler was used during the development phase. 

## Installation of the C Compiler
This code was written in C and the Windows operating system was used during the development. 
MinGW (Minimalist GNU for Windows) freeware which is the collection of GNU Compiler collection (GCC)
was used to install the gcc compiler for windows. 

The gcc compiler was used to compile and execute this C code.

If you want to replicate the environment, you can download the MinGW from 
[MinGW](https://sourceforge.net/projects/mingw/)

After installing the MinGW, you can select the gcc compiler and the same will get installed.

Add the gcc to the windows environment variable so that the gcc command can be invoked from anywhere. 

## Contents
- demo2.c : The main assembler code responsible for translating assembly instructions into machine code.
- input.s: A sample file containing RISC-V assembly instructions to be processed by the assembler.
- output.hex: The output file that contains the translated machine code in hexadecimal format.
To know the details of each function, variables and the flow of code execution, refer the report.pdf

## Executing the program
In the command prompt, type the a.out or the executable file name to execute this program. 
Upon successful execution of the program, you will get an output file named output.hex with the translated machine codes in hexadecimal format.

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
     <td>`imm[11:0]`</td>
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
     <td>`imm[11:5]`</td>
     <td>`rs2`</td>
     <td>`rs1`</td>
     <td>`funct3`</td>
     <td>`imm[4:0]'</td>
     <td>`opcode`</td>
  </tr>
</table>

- imm[11:5]: The upper 7 bits of the immediate value.
- rs2: A 5-bit field representing the second source register.
- rs1: A 5-bit field representing the first source register.
- funct3: A 3-bit function code that specifies the operation.
- imm[4:0]: The lower 5 bits of the immediate value.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, S format: 0100011

### B-Format

<table>
  <tr>
    <th>31</th>
    <th>30-25</th>
    <th>24-20</th>
    <th>19-15</th>
    <th>14-12</th>
    <th>11-8</th>
    <th>7</th>
    <th>6-0</th>
  </tr>
  <tr>
     <td>`imm[12]`</td>
     <td>`imm[10:5]`</td>
     <td>`rs2`</td>
     <td>`rs1`</td>
     <td>`funct3`</td>
     <td>`imm[4:1]`</td>
     <td>`imm[11]`</td>
     <td>`opcode`</td>
  </tr>
</table>

- imm[12|10:5]: The concatenated 7 bits of the immediate value.
- rs2: A 5-bit field representing the second source register.
- rs1: A 5-bit field representing the first source register.
- funct3: A 3-bit function code that specifies the operation.
- imm[4:1|11]: The concatenated 5 bits of the immediate value and the 11th bit.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, B format: 1100011

### U-Format

<table>
  <tr>
    <th>31-12</th>
    <th>11-7</th>
    <th>6-0</th>
  </tr>
  <tr>
     <td>`imm[31:12]`</td>
     <td>`rd`</td>
     <td>`opcode`</td>
  </tr>
</table>

- imm[31:12]: A 20-bit immediate value that is shifted left by 12 bits and added to the PC.
- rd: A 5-bit field representing the destination register.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, U format: 0110111[For lui] , 0010111 [For auipc]

### J-Format

<table>
  <tr>
    <th>31</th>
    <th>30-21</th>
    <th>20</th>
    <th>19-12</th>
    <th>11-7</th>
    <th>6-0</th>
  </tr>
  <tr>
     <td>`imm[20]`</td>
     <td>`imm[10:1]`</td>
     <td>`imm[11]`</td>
     <td>`imm[19:12]`</td>
     <td>`rd`</td>
     <td>`opcode`</td>
  </tr>
</table>

- imm[20|10:1|11|19:12]: The concatenated 20-bit immediate value used for jumps.
- rd: A 5-bit field representing the destination register.
- opcode: A 7-bit operation code that specifies the type of instruction. Here, J format: 1101111
