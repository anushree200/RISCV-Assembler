#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "instr_J.h"

void process_instructionJ(char *line, unsigned int opcode, int labeladd, int curaddr, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg1[10], label[50];
    char extra[10];
    int operand_count = sscanf(line, "%*s %s %s %s", reg1, label, extra);
    if (operand_count > 2)
    {
        fprintf(fileo, "Error: Instruction has too many operands. Expected at most 2, but got %d in line %d\n", operand_count, n);
        fclose(fileo);
        return;
    }

    // Check for too few operands
    if (operand_count < 2)
    {
        fprintf(fileo, "Error: Instruction parsing failed, expected 2 operands but got %d in line %d\n", operand_count, n);
        fclose(fileo);
        return;
    }
    reg1[strcspn(reg1, ",")] = '\0';
    int rd = get_register_number(reg1);
    if (labeladd == -1)
    {
        fprintf(fileo, "Error: Undefined label %s\n", label);
        fclose(fileo);
        return;
    }

    int offset = (labeladd - curaddr); // Adjust for instruction size
    // Construct the instruction
    if (offset < -2097152 || offset > 2097151)
    {
        fprintf(fileo, "Error: J-Type immediate value out of range: %d in line %d\n", offset, n);
        fclose(fileo);
        return;
    }

    unsigned int imm_20 = (offset >> 20) & 0x1;     // imm[20]
    unsigned int imm_19_12 = (offset >> 12) & 0xFF; // imm[19:12]
    unsigned int imm_11 = (offset >> 11) & 0x1;     // imm[11]
    unsigned int imm_10_1 = (offset >> 1) & 0x3FF;  // imm[10:1]

    unsigned int instruction = (imm_20 << 31) | (imm_19_12 << 12) | (imm_11 << 20) |
                               (imm_10_1 << 21) | (rd << 7) | opcode;

    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}