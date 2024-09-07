#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "instr_B.h"
#include "register_no.h"

void process_instructionB(char *line, unsigned int funct3, int labeladd, int curaddr, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg1[10], reg2[10], label[50];
    char extra[10];
    int operand_count = sscanf(line, "%*s %s %s %s %s", reg1, reg2, label, extra);
    if (operand_count > 3)
    {
        fprintf(fileo, "Error: Instruction has too many operands. Expected at most 3, but got %d in line %d\n", operand_count, n);
        fclose(fileo);
        return;
    }

    // Check for too few operands
    if (operand_count < 3)
    {
        fprintf(fileo, "Error: Instruction parsing failed, expected 3 operands but got %d in line %d\n", operand_count, n);
        fclose(fileo);
        return;
    }
    reg1[strcspn(reg1, ",")] = '\0';
    reg2[strcspn(reg2, ",")] = '\0';
    int rs1 = get_register_number(reg1);
    int rs2 = get_register_number(reg2);
    if (labeladd == -1)
    {
        fprintf(fileo, "Error: Undefined label %s\n", label);
        fclose(fileo);
        return;
    }
    int calculatedOffset = (labeladd - curaddr) / 4;
    if (calculatedOffset < -2048 || calculatedOffset > 2047)
    {
        fprintf(fileo, "Error: B-Type immediate value out of range: %d in line %d\n", calculatedOffset, n);
        fclose(fileo);
        return;
    }
    unsigned int imm_12 = (calculatedOffset >> 12) & 0x1;   // imm[12]
    unsigned int imm_10_5 = (calculatedOffset >> 5) & 0x3F; // imm[10:5]
    unsigned int imm_4_1 = (calculatedOffset << 1) & 0xF;   // 4 >> 1 gives 2
    unsigned int imm_11 = (calculatedOffset >> 11) & 0x1;   // imm[11]

    // Construct the instruction
    unsigned int instruction = (imm_12 << 31) | (imm_11 << 7) |
                               (imm_10_5 << 25) | (imm_4_1 << 8) |
                               (rs2 << 20) | (rs1 << 15) |
                               (funct3 << 12) | 0b1100011;

    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}
