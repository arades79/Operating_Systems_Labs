/**
 * @file processor.h
 * @author Skyelar Craver & Steven Pitts
 * @brief helpers and definitions for processor simulator
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#pragma once

#include <stdlib.h>
#include <stdio.h>

// static definitions of memory offsets
static const uint16_t INSTRUCTION_ADDRESS_OFFSET = 0x940;
static const uint16_t DATA_ADDRESS_OFFSET = 0x300;

// definitions of valid opcodes for instructions
enum Opcodes
{
    HALT = 0,
    LOAD_ACC = 0b0001,
    STORE_ACC = 0b0010,
    ADD_ACC = 0b0101,
    LOAD_IO = 0b0011,
    SUB_ACC = 0b0110,
    STORE_IO = 0b0111,
    MUL_ACC = 0b1000,
    DIV_ACC = 0b1001,
    INTERRUPT = 0b0100
};

// bitfield packed 16-bit struct to hold strongly typed Instructions
typedef struct Instruction
{
    uint16_t opcode : 4;
    uint16_t address : 12;
} Instruction;

// generic device struct
typedef struct IODevice
{
    int16_t buffer[10];
    uint16_t buffer_index;
} IODevice;

// part 1 program data
// static Instruction instruction_memory[16] =
// {
//     {0x1,0x940},
//     {0x5,0x941},
//     {0x2,0x941}
// };

// part 2 + bonus program data
static Instruction instruction_memory[16] =
    {
        {0x3, 0x005}, // Load AC from device 5.
        {0x5, 0x940}, // Add AC with contents of memory location 940.
        {0x9, 0x941}, // Divide AC by contents of memory location 941.
        {0x7, 0x006}, // Store AC to device 6.
        {0x2, 0x942}, // Store AC to memory location 942.
        {0x3, 0x005}, // Load AC from device 5.
        {0x8, 0x942}, // Multiply AC with contents of memory location 942.
        {0x6, 0x943}, // Subtract contents of memory location 943 from the AC.
        {0x2, 0x944}, // Store AC to memory location 944.
        {0x7, 0x006}, // Store AC to device 6.
};

// part 3 program data
// static Instruction instruction_memory[16] =
// {
//     {0x3, 0x005},  // Load AC from device 5.
//     {0x4, 0x000},  // insterted interupt
//     {0x5, 0x940},  // Add AC with contents of memory location 940.
//     {0x9, 0x941},  // Divide AC by contents of memory location 941.
//     {0x7, 0x006},  // Store AC to device 6.
//     {0x4, 0x000},  // insterted interupt
//     {0x2, 0x942},  // Store AC to memory location 942.
//     {0x3, 0x005},  // Load AC from device 5.
//     {0x4, 0x000},  // insterted interupt
//     {0x8, 0x942},  // Multiply AC with contents of memory location 942.
//     {0x6, 0x943},  // Subtract contents of memory location 943 from the AC.
//     {0x2, 0x944},  // Store AC to memory location 944.
//     {0x7, 0x006},  // Store AC to device 6.
//     {0x4, 0x000},  // insterted interupt
// };

// part 1 data pre-fill
// static uint16_t data_memory[16] =
// {
//     3,
//     2
// };

// part 2+ data pre-fill
static int16_t data_memory[16] =
    {
        3,
        5,
        0,
        10};

// attached device array (only 5 and 6 used)
static IODevice devices[10];

// prints static array contents
void PrintMemory()
{
    printf("Instruction Memory");
    for (size_t i = 0; i < 16; i++)
    {
        if (!(i % 8))
            printf("\n");
        printf("%x:%01x%03x\t",
               i + 0x300,
               instruction_memory[i].opcode,
               instruction_memory[i].address);
    }
    printf("\nData Memory\n");
    for (size_t i = 0; i < 16; i++)
    {
        printf("%x:%d\t", i + INSTRUCTION_ADDRESS_OFFSET, data_memory[i]);
    }
    printf("\nDevice 5\t");
    for (size_t i = 0; i < 9; i++)
    {
        printf("%d\t", devices[5].buffer[i]);
    }
    printf("\nDevice 6\t");
    for (size_t i = 0; i < 9; i++)
    {
        printf("%d\t", devices[6].buffer[i]);
    }
    printf("\n");
}

// inserts interrupts for bonus, slides array after insertion
void InsertInterrupt(uint16_t index)
{
    Instruction interupt = {0x4, 0x000};
    Instruction existing_instruction = instruction_memory[index];
    instruction_memory[index] = interupt;
    for (size_t i = index + 1; i < 15; i++)
    {
        Instruction last_instruction = instruction_memory[i];
        instruction_memory[i] = existing_instruction;
        existing_instruction = last_instruction;
    }
}
