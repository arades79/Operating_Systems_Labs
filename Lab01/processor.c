/**
 * @file processor.c
 * @author Skyelar Craver & Steven Pitts
 * @brief simple processor simulator
 * @version 0.1
 * @date 2019-02-09
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#include "processor.h"

int main(int argc, char const *argv[])
{
    // setup block for initializing
    srand(time(NULL)); // seed the PRNG
    uint8_t instruction_index = 0;
    int16_t accumulator = 0;
    uint16_t data_index;
    uint16_t buffer_index;
    uint16_t step_count = 1;

    devices[5].buffer[0] = 2;
    devices[5].buffer[1] = 13;

    // messages to dynamically build printf statements
    const char execute_context_descriptions[][100] =
        {
            "HALT",
            "Load AC from memory location",
            "Store AC to memory location",
            "Load to AC from device",
            "INTERRUPT", "Add to AC from memory location",
            "Subtract from AC from memory location",
            "Store AC to device", "Multiply by AC from memory location",
            "Divide by AC from memory location"};
    // Format with execute context description and memory location
    const char execute_context[] = "Execute the instruction and increment the PC: %s %x\n";
    // Format with memory location
    const char fetch_context[] = "Fetch instruction from memory location %x\n";

    printf("BEGIN SIMULATION\n");
    PrintMemory();

    // main loop
    // runs until a HALT (opcode 0)
    while (instruction_memory[instruction_index].opcode)
    {
        Instruction current_instruction = instruction_memory[instruction_index];
        printf("step: %u\n", step_count++);
        printf(fetch_context, instruction_index + 0x300);
        // print register contents
        printf("AC: \t%d\tPC:\t%x\tIR:\t%01x%03x\n",
               accumulator,
               (instruction_index + 0x300),
               current_instruction.opcode,
               current_instruction.address);
        instruction_index++;

        printf("step: %u\n", step_count++);
        if (current_instruction.opcode <= 10)
        {
            printf(execute_context,
                   execute_context_descriptions[current_instruction.opcode],
                   current_instruction.address);
        }
        else
        {
            printf(execute_context, execute_context_descriptions[0],
                   current_instruction.address);
            return 0;
        }

        // remove offset
        data_index = current_instruction.address - INSTRUCTION_ADDRESS_OFFSET;
        buffer_index = devices[current_instruction.address].buffer_index;

        // switch on opcode to determine operation
        switch (current_instruction.opcode)
        {
        case LOAD_ACC:
            accumulator = data_memory[data_index];
            break;
        case STORE_ACC:
            data_memory[data_index] = accumulator;
            break;
        case LOAD_IO:
            accumulator = devices[current_instruction.address].buffer[buffer_index];
            devices[current_instruction.address].buffer_index++;
            //InsertInterrupt(instruction_index + ((rand() % 2) + 1));
            break;
        case STORE_IO:
            devices[current_instruction.address].buffer[buffer_index] = accumulator;
            devices[current_instruction.address].buffer_index++;
            //InsertInterrupt(instruction_index + ((rand() % 2) + 1));
            break;
        case ADD_ACC:
            accumulator += data_memory[data_index];
            break;
        case SUB_ACC:
            accumulator -= data_memory[data_index];
            break;
        case MUL_ACC:
            accumulator *= data_memory[data_index];
            break;
        case DIV_ACC:
            accumulator /= data_memory[data_index];
            break;
        case INTERRUPT:
            break;
        default:
            printf("Unsupported instruction %01x called\n",
                   current_instruction.opcode);
            break;
        }
        // print register contents
        printf("AC: \t%d\tPC:\t%x\tIR:\t%01x%03x\n",
               accumulator,
               (instruction_index + 0x300),
               current_instruction.opcode,
               current_instruction.address);
    }

    PrintMemory();
    printf("END SIMULATION\n");
    return 0;
}
