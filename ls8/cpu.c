#include "cpu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DATA_LEN 6

/**
 * Load the binary bytes from a .ls8 source file into a RAM array
 */
void cpu_ram_write(struct cpu *cpu, unsigned char adrs, unsigned char value){
    cpu->ram[adrs] = value;
}

void cpu_load(struct cpu *cpu, char *load_program)
{
    char data[DATA_LEN] = {
        // From print8.ls8
        0b10000010, // LDI R0,8
        0b00000000,
        0b00001000,
        0b01000111, // PRN R0
        0b00000000,
        0b00000001  // HLT
    };
    
   int address = 0;
//
//    for (int i = 0; i < DATA_LEN; i++) {
//        cpu->ram[address++] = data[i];
//    }
    
    // TODO: Replace this with something less hard-coded
    
    FILE *fp;
    char line[1024];
 
    
    fp = fopen(load_program, "r");
    
    if (fp == NULL) {
        fprintf(stderr,"comp: error opening file\n");
        exit(2);
    }
    
    while (fgets(line, 1024, fp) != NULL) {
        char *endptr;
        
        unsigned int val = strtoul(line, &endptr, 2) & 0xFF ;
        
        if (endptr == line) {
            //printf("Found no digits\n");
            continue;
        }
        
        //printf("%u\n", val);
        
        cpu_ram_write(cpu, address ++, val) ;
    }
    
    fclose(fp);
    
    
}

/**
 * ALU
 */
void alu(struct cpu *cpu, enum alu_op op, unsigned char regA, unsigned char regB)
{
    switch (op) {
        case ALU_MUL:
            // TODO
            break;
            
            // TODO: implement more ALU ops
    }
}

unsigned char cpu_ram_read(struct cpu *cpu, unsigned char adrs) {
    return cpu->ram[adrs];
}

/**
 * Run the CPU
 */
void cpu_run(struct cpu *cpu)
{
    int running = 1; // True until we get a HLT instruction
    
    while (running) {
        // TODO
        unsigned char val = cpu_ram_read(cpu, cpu->pc) ;
        // 1. Get the value of the current instruction (in address PC).
        // 2. Figure out how many operands this next instruction requires
        unsigned char operandA = cpu_ram_read(cpu, cpu->pc + 1);
        unsigned char operandB = cpu_ram_read(cpu, cpu->pc + 2);
        // 3. Get the appropriate value(s) of the operands following this instruction
        // 4. switch() over it to decide on a course of action.
        
        switch (val) {
            case LDI:
                cpu->reg[operandA] = operandB;
                cpu->pc += 3;
                break;
            case PRN:
                printf("%d\n", cpu->reg[operandA]);
                cpu->pc += 2;
                break;
            case HLT:
                running = 0;
                break;
            default:
                printf("Unknown instruction at %d: %d\n", cpu->pc, val);
                exit(1);
        }
        
        // 5. Do whatever the instruction should do according to the spec.
        // 6. Move the PC to the next instruction.
    }
}

/**
 * Initialize a CPU struct
 */
void cpu_init(struct cpu *cpu)
{
    cpu->pc = 0;
    memset(cpu->reg, 0, sizeof(unsigned char)*8);
    memset(cpu->ram, 0, sizeof(unsigned char)*256);
    // TODO: Initialize the PC and other special registers
}



