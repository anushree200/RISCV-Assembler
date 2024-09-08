#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "instr_R.h"
#include "instr_IS.h"
#include "instr_U.h"
#include "instr_B.h"
#include "instr_J.h"
#include "register_no.h"
void process_instructionR(char *line, unsigned int funct3, unsigned int funct7, int n);
void process_instructionIS(char *line, unsigned int funct3, unsigned int opcode, int n);
void SRAI_IS(char *line, unsigned int funct3, unsigned int opcode, int n);
void process_instructionB(char *line, unsigned int funct3, int labeladd, int curaddr, int n);
void process_instructionJ(char *line, unsigned int opcode, int labeladd, int curaddr, int n);
void process_instructionU(char *line, int n);
void process_instr(FILE *file);
int get_register_number(const char *reg);

#define MAX_LABELS 100
#define MAX_LABEL_LENGTH 50

typedef struct
{
    char label[MAX_LABEL_LENGTH];
    int address;
} Label;

Label labels[MAX_LABELS];
int label_count = 0;
int dup[MAX_LABELS];
int j = -1;
void add_label(const char *label_name, int curaddr)
{
    if (label_count < MAX_LABELS)
    {
        for (int i = 0; i < label_count; i++)
        {
            if (strcmp(labels[i].label, label_name) == 0)
            {
                j += 1;
                dup[j] = curaddr;
                return;
            }
        }
        strcpy(labels[label_count].label, label_name);
        labels[label_count].address = curaddr;
        label_count++;
    }
}

int find_label_address(const char *label)
{
    for (int i = 0; i < label_count; i++)
    {
        if (strcmp(labels[i].label, label) == 0)
        {
            return labels[i].address;
        }
    }
    return -1; 
}

int main()
{
    FILE *file = fopen("input.s", "r");
    if (file == NULL)
    {
        printf("Error opening file\n");
        return 1;
    }
    FILE *fileo = fopen("output.hex", "w");
    if (fileo == NULL)
    {
        printf("Error opening file for writing!\n");
        fclose(file);
        return 1;
    }
    fclose(fileo);
    process_instr(file);
    fclose(file);
    return 0;
}

void process_instr(FILE *file)
{
    char line[256];
    char instruction[10];
    char label[50];
    int n = 1;
    int curaddr = 0;
    int add;
    bool labelerror = false;
    while (fgets(line, sizeof(line), file))
    {
        char found_label[50];
        if (sscanf(line, "%[^:]:", found_label) == 1)
        {
            add_label(found_label, curaddr);
        }
        curaddr += 4;
    }

    rewind(file);
    curaddr = 0;

    while (fgets(line, sizeof(line), file))
    {
        char instruction[10];
        char label[50];
        sscanf(line, "%s", instruction);
        if (n != 1)
        {
            curaddr += 4;
        }

        if (strchr(instruction, ':'))
        {
            sscanf(line, "%[^:]: %[^\n]", label, line);
            for (int i = 0; i < j + 1; i++)
            {
                if (dup[i] == curaddr)
                {
                    FILE *fileo = fopen("output.hex", "a");
                    if (fileo == NULL)
                    {
                        printf("Error opening file for appending!\n");
                        return;
                    }
                    fprintf(fileo, "This is a multiple label error for label %s in line number %d \n", label, n);
                    labelerror = true;
                    fclose(fileo);
                    continue;
                }
            }
        }
        if (labelerror)
        {
            continue;
        }
        sscanf(line, "%s", instruction);
        if (strcmp(instruction, "sra") == 0)
        {
            process_instructionR(line, 0b101, 0b0100000, n);
        }
        else if (strcmp(instruction, "sub") == 0)
        {
            process_instructionR(line, 0b000, 0b0100000, n);
        }
        else if (strcmp(instruction, "or") == 0)
        {
            process_instructionR(line, 0b110, 0b0000000, n);
        }
        else if (strcmp(instruction, "sll") == 0)
        {
            process_instructionR(line, 0b001, 0b0000000, n);
        }
        else if (strcmp(instruction, "and") == 0)
        {
            process_instructionR(line, 0b111, 0b0000000, n);
        }
        else if (strcmp(instruction, "srl") == 0)
        {
            process_instructionR(line, 0b101, 0b0000000, n);
        }
        else if (strcmp(instruction, "add") == 0)
        {
            process_instructionR(line, 0b000, 0b0000000, n);
        }
        else if (strcmp(instruction, "xor") == 0)
        {
            process_instructionR(line, 0b100, 0b0000000, n);
        }
        else if (strcmp(instruction, "addi") == 0)
        {
            process_instructionIS(line, 0b000, 0b0010011, n);
        }
        else if (strcmp(instruction, "andi") == 0)
        {
            process_instructionIS(line, 0b111, 0b0010011, n);
        }
        else if (strcmp(instruction, "ori") == 0)
        {
            process_instructionIS(line, 0b110, 0b0010011, n);
        }
        else if (strcmp(instruction, "xori") == 0)
        {
            process_instructionIS(line, 0b100, 0b0010011, n);
        }
        else if (strcmp(instruction, "slli") == 0)
        {
            process_instructionIS(line, 0b001, 0b0010011, n);
        }
        else if (strcmp(instruction, "srli") == 0)
        {
            process_instructionIS(line, 0b101, 0b0010011, n);
        }
        else if (strcmp(instruction, "srai") == 0)
        {
            SRAI_IS(line, 0b101, 0b0010011, n);
        }

        else if (strcmp(instruction, "ld") == 0)
        {
            process_instructionIS(line, 0b011, 0b0000011, n);
        }
        else if (strcmp(instruction, "lw") == 0)
        {
            process_instructionIS(line, 0b010, 0b0000011, n);
        }
        else if (strcmp(instruction, "lh") == 0)
        {
            process_instructionIS(line, 0b001, 0b0000011, n);
        }
        else if (strcmp(instruction, "lb") == 0)
        {
            process_instructionIS(line, 0b000, 0b0000011, n);
        }
        else if (strcmp(instruction, "lwu") == 0)
        {
            process_instructionIS(line, 0b110, 0b0000011, n);
        }
        else if (strcmp(instruction, "lhu") == 0)
        {
            process_instructionIS(line, 0b101, 0b0000011, n);
        }
        else if (strcmp(instruction, "lbu") == 0)
        {
            process_instructionIS(line, 0b100, 0b0000011, n);
        }

        // s format
        else if (strcmp(instruction, "sd") == 0)
        {
            process_instructionIS(line, 0b011, 0b0100011, n);
        }
        else if (strcmp(instruction, "sw") == 0)
        {
            process_instructionIS(line, 0b010, 0b0100011, n);
        }
        else if (strcmp(instruction, "sh") == 0)
        {
            process_instructionIS(line, 0b001, 0b0100011, n);
        }
        else if (strcmp(instruction, "sb") == 0)
        {
            process_instructionIS(line, 0b000, 0b0100011, n);
        }
        else if (strcmp(instruction, "jalr") == 0)
        {
            process_instructionIS(line, 0b000, 0b1100111, n);
        }
        // b
        else if (strcmp(instruction, "beq") == 0)
        {
            sscanf(line, "%*s %*s %*s %s", label);
            add = find_label_address(label);
            process_instructionB(line, 0b000, add, curaddr, n);
        }
        else if (strcmp(instruction, "bne") == 0)
        {
            sscanf(instruction, "%*s %*s %*s %s", label);
            add = find_label_address(label);

            process_instructionB(line, 0b001, add, curaddr, n);
        }
        else if (strcmp(instruction, "blt") == 0)
        {
            sscanf(instruction, "%*s %*s %*s %s", label);
            add = find_label_address(label);
            process_instructionB(line, 0b100, add, curaddr, n);
        }
        else if (strcmp(instruction, "bge") == 0)
        {
            sscanf(instruction, "%*s %*s %*s %s", label);
            add = find_label_address(label);

            process_instructionB(line, 0b101, add, curaddr, n);
        }
        else if (strcmp(instruction, "bltu") == 0)
        {
            sscanf(instruction, "%*s %*s %*s %s", label);
            add = find_label_address(label);
            process_instructionB(line, 0b110, add, curaddr, n);
        }
        else if (strcmp(instruction, "bgeu") == 0)
        {
            sscanf(instruction, "%*s %*s %*s %s", label);
            add = find_label_address(label);
            process_instructionB(line, 0b111, add, curaddr, n);
        }
        // j
        else if (strcmp(instruction, "jal") == 0)
        {
            sscanf(line, "%*s %*s %s", label);
            add = find_label_address(label);
            process_instructionJ(line, 0b1101111, add, curaddr, n);
        }
        // u
        else if (strcmp(instruction, "lui") == 0)
        {
            process_instructionU(line, n);
        }
        else
        {
            printf("Invalid Instruction, in line no. %d", n);
        }
        n += 1;
    }
}
