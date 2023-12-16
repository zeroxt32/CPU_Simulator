#include <stdio.h>
#include "cpu.h"
#include "memory.h"
#include "scheduler.h"
#include "smm.h"
// Define CPU registers
int Base;
int PC;
int IR0;
int IR1;
int AC;
int MAR;
int MBR;

// Function to fetch an instruction from memory
void fetch_instruction(int addr) {
    // Read the instruction from memory at the specified address
    int current_pid = get_current_process_pid();
    int* instruction = mem_read(addr, current_pid);

    // Check if the instruction is valid (not NULL)
    if (instruction != NULL) {
        IR0 = instruction[0]; // OP Code
        IR1 = instruction[1]; // Argument
    } else {
        // Handle error: Invalid instruction or memory address
        printf("Error: Invalid instruction or memory address at PC=%d\n", PC);
        deallocate(current_pid);
        remove_process(current_pid);
        //continue executing
        IR0 = 0;
        IR1 = 1;

        
    }
}

// Function to execute the instruction in IR0 and IR1 registers
void execute_instruction() {
    switch (IR0) {
        case 0: // exit
            printf("Program execution completed.\n");
            break;
        case 1: // load_const
            AC = IR1; // Load the argument into the accumulator (AC)
            
            break;
        case 2: // move_from_mbr
            AC = MBR; // Move data from MBR to AC
            
            break;
        case 3: // move_from_mar
            AC = MAR; // Move data from MAR to AC
        
            break;
        case 4: // move_to_mbr
            MBR = AC; // Move data from AC to MBR
            break;
        case 5: // move_to_mar
            MAR = AC; // Move data from AC to MAR
            break;
        case 6: // load_at_addr
            // Read data from memory at the address in MAR into MBR
            mem_read(MAR, get_current_process_pid());
            printf("Last Read /n");
            break;
        case 7: // write_at_addr
            // Write data from MBR to memory at the address in MAR
            mem_write(MAR, &MBR, get_current_process_pid());
          
            break;
        case 8: // add
            AC += MBR; // Add data from MBR to AC
            break;
        case 9: // multiply
            AC *= MBR; // Multiply AC by data in MBR
            break;
        case 10: // and
            AC = AC && MBR; // Perform logical AND operation and store in AC
            break;
        case 11: // or
            AC = AC || MBR; // Perform logical OR operation and store in AC
            break;
        case 12: // ifgo
            if (AC != 0) {
                //PC = IR1 - 1; // Jump to the specified address (adjusted by -1)
                PC = IR1;
            }
            break;
        case 13: // sleep
            // Do nothing (placeholder for sleep operation)
            break;
        default:
            // Handle error: Invalid OP Code
            printf("Error: Invalid OP Code %d at PC=%d\n", IR0, PC);
            break;
    }
}

// Function to calculate true memory address
int mem_address(int l_addr) {
    //printf("Memory address of base is %d\n", Base);
    return Base + l_addr;
}

// Function to simulate a clock cycle
int clock_cycle() {
    fetch_instruction(mem_address(PC)); // Fetch the instruction at the current PC
    PC++; // Increment PC for the next instruction

    execute_instruction(); // Execute the fetched instruction

    return (IR0 == 0) ? 0 : 1; // Return 0 if exit command encountered, 1 otherwise
}

// Function for context switching
register_struct context_switch(register_struct new_vals) {
    // Create a copy of the current register values
    register_struct old_vals;
    old_vals.base = Base;
    old_vals.pc = PC;
    old_vals.ir0 = IR0;
    old_vals.ir1 = IR1;
    old_vals.ac = AC;
    old_vals.mar = MAR;
    old_vals.mbr = MBR;

    // Set the new register values
    Base = new_vals.base;
    PC = new_vals.pc;
    IR0 = new_vals.ir0;
    IR1 = new_vals.ir1;
    AC = new_vals.ac;
    MAR = new_vals.mar;
    MBR = new_vals.mbr;

    // Return the old register values
    return old_vals;
}

