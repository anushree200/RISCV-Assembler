#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
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
    return -1; // Label not found
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
        curaddr += 4; // Increment address for each instruction
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
        printf(" %d%s ", curaddr, instruction);

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
                    fprintf(fileo, "This is a multiple label error for label %s in line number %d\n", label,n);
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
        printf(" %d%s ", curaddr, instruction);
        if (strcmp(instruction, "sra") == 0)
        {
            process_instructionR(line, 0b101, 0b0100000, n); // funct3 for SRA, funct7 for SRA
        }
        else if (strcmp(instruction, "sub") == 0)
        {
            process_instructionR(line, 0b000, 0b0100000, n); // funct3 for SUB
        }
        else if (strcmp(instruction, "or") == 0)
        {
            process_instructionR(line, 0b110, 0b0000000, n); // funct3 for OR
        }
        else if (strcmp(instruction, "sll") == 0)
        {
            process_instructionR(line, 0b001, 0b0000000, n); // funct3 for SLL
        }
        else if (strcmp(instruction, "and") == 0)
        {
            process_instructionR(line, 0b111, 0b0000000, n); // funct3 for AND
        }
        else if (strcmp(instruction, "srl") == 0)
        {
            process_instructionR(line, 0b101, 0b0000000, n); // funct3 for SRL
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
        printf(" %d%s ", curaddr, instruction);
        n += 1;
    }
}

void process_instructionR(char *line, unsigned int funct3, unsigned int funct7, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg1[10], reg2[10], reg3[10];
    char extra[10];
    int operand_count = sscanf(line, "%*s %s %s %s %s", reg1, reg2, reg3, extra);

    // Check for too many operands
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
    reg3[strcspn(reg3, ",")] = '\0';

    int rd = get_register_number(reg1);
    int rs1 = get_register_number(reg2);
    int rs2 = get_register_number(reg3);
    unsigned int opcode = 0b0110011;
    unsigned int instruction = (funct7 << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;

    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}

void process_instructionIS(char *line, unsigned int funct3, unsigned int opcode, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg1[10], reg2[10], imm[10];
    char extra[10];
    unsigned int instruction;
    int immI;
    if (opcode == 0b0010011)
    {
        int operand_count = sscanf(line, "%*s %s %s %s %s", reg1, reg2, imm, extra);
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
        if (immI < -2048 || immI > 2047)
        { // 12-bit signed immediate range
            fprintf(fileo, "Error: Immediate value %d out of range for instruction in line %d\n", immI, n);
            fclose(fileo);
            return;
        }
        reg1[strcspn(reg1, ",")] = '\0';
        reg2[strcspn(reg2, ",")] = '\0';
        immI = strtol(imm, NULL, 0);
        immI &= 0xFFF;
        int rd = get_register_number(reg1);
        int rs1 = get_register_number(reg2);
        instruction = (immI << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
    }
    else if (opcode == 0b0000011)
    {
        int operand_count = sscanf(line, "%*s %[^,], %d(%[^)]) %s", reg1, &immI, reg2, extra);
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
        if (immI < -2048 || immI > 2047)
        { // 12-bit signed immediate range
            fprintf(fileo, "Error: Immediate value %d out of range for instruction in line %d\n", immI, n);
            fclose(fileo);
            return;
        }
        immI &= 0xFFF;
        int rd = get_register_number(reg1);
        int rs1 = get_register_number(reg2);
        instruction = (immI << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
    }
    else if (opcode == 0b0100011)
    {
        int operand_count = sscanf(line, "%*s %[^,], %d(%[^)]) %s", reg2, &immI, reg1, extra);
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
        if (immI < -2048 || immI > 2047)
        { // 12-bit signed immediate range
            fprintf(fileo, "Error: Immediate value %d out of range for instruction in line %d\n", immI, n);
            fclose(fileo);
            return;
        }
        int imm_high = (immI >> 5) & 0x7F;
        int imm_low = immI & 0x1F;
        int rs1 = get_register_number(reg1);
        int rs2 = get_register_number(reg2);
        instruction = (imm_high << 25) | (rs2 << 20) | (rs1 << 15) | (funct3 << 12) | (imm_low << 7) | opcode;
    }
    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}

void SRAI_IS(char *line, unsigned int funct3, unsigned int opcode, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg1[10], reg2[10], imm[10];
    char extra[10];
    unsigned int instruction;
    int immI;
    int operand_count = sscanf(line, "%*s %s %s %s %s", reg1, reg2, imm, extra);
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
    immI = strtol(imm, NULL, 0);
    if (immI < 0 || immI > 31)
    {
        fprintf(fileo, "Error: Immediate value %d for SRAI is out of range (0-31) in line %d\n", immI, n);
        fclose(fileo);
        return;
    }
    int rd = get_register_number(reg1);
    int rs1 = get_register_number(reg2);
    unsigned int shamt = immI & 0x1F;    // Lower 5 bits for shift amount
    unsigned int shamt_high = 0b0100000; // Set the upper bits to indicate SRAI
    instruction = (shamt_high << 25) | (shamt << 20) | (rs1 << 15) | (funct3 << 12) | (rd << 7) | opcode;
    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}

void process_instructionU(char *line, int n)
{
    FILE *fileo = fopen("output.hex", "a");
    if (fileo == NULL)
    {
        printf("Error opening file for appending!\n");
        return;
    }
    char reg[10];
    char extra[10];
    unsigned int imm;
    int operand_count = sscanf(line, "%*s %s %x %s", reg, &imm, extra);
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
    reg[strcspn(reg, ",")] = '\0';

    int rd = get_register_number(reg);
    if (imm > 0xFFFFF)
    {
        fprintf(fileo, "Immediate value %u for %s is out of range\n", imm, line);
        fclose(fileo);
        return;
    }

    unsigned int instruction = (imm << 12) | (rd << 7) | 0b0110111;

    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}

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
    printf(" ####### %d  %d  ######", labeladd, curaddr);
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
                                                            // imm[4:1]
    unsigned int imm_11 = (calculatedOffset >> 11) & 0x1;   // imm[11]

    // Construct the instruction
    unsigned int instruction = (imm_12 << 31) | (imm_11 << 7) |
                               (imm_10_5 << 25) | (imm_4_1 << 8) |
                               (rs2 << 20) | (rs1 << 15) |
                               (funct3 << 12) | 0b1100011;

    fprintf(fileo, "%08X\n", instruction);
    fclose(fileo);
}

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
    printf(" ####### %d  %d  ######", labeladd, curaddr);
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

int get_register_number(const char *reg)
{
    // Register name and their corresponding numbers
    struct
    {
        const char *name;
        int number;
    } reg_map[] = {
        {"zero", 0},
        {"x0", 0},
        {"x1", 1},
        {"x2", 2},
        {"x3", 3},
        {"x4", 4},
        {"x5", 5},
        {"x6", 6},
        {"x7", 7},
        {"x8", 8},
        {"x9", 9},
        {"x10", 10},
        {"x11", 11},
        {"x12", 12},
        {"x13", 13},
        {"x14", 14},
        {"x15", 15},
        {"x16", 16},
        {"x17", 17},
        {"x18", 18},
        {"x19", 19},
        {"x20", 20},
        {"x21", 21},
        {"x22", 22},
        {"x23", 23},
        {"x24", 24},
        {"x25", 25},
        {"x26", 26},
        {"x27", 27},
        {"x28", 28},
        {"x29", 29},
        {"x30", 30},
        {"x31", 31},
        {"t0", 5},
        {"t1", 6},
        {"t2", 7},
        {"t3", 28},
        {"t4", 29},
        {"t5", 30},
        {"t6", 31},
        {"s0", 8},
        {"s1", 9},
        {"s2", 18},
        {"s3", 19},
        {"s4", 20},
        {"s5", 21},
        {"s6", 22},
        {"s7", 23},
        {"s8", 24},
        {"s9", 25},
        {"s10", 26},
        {"s11", 27},
        {"a0", 10},
        {"a1", 11},
        {"a2", 12},
        {"a3", 13},
        {"a4", 14},
        {"a5", 15},
        {"a6", 16},
        {"a7", 17},
    };

    // Calculate the size of the reg_map array
    size_t num_regs = sizeof(reg_map) / sizeof(reg_map[0]);

    // Loop through the register map to find a match
    for (size_t i = 0; i < num_regs; i++)
    {
        if (strcmp(reg, reg_map[i].name) == 0)
        {
            return reg_map[i].number; // Return the corresponding register number
        }
    }
    return -1; // Error: invalid register
}
