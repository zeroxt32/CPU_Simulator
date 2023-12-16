#include <stdio.h>
#include "memory.h"
#include "smm.h"
#include "scheduler.h"
// Define the memory representation
int memory[MAX_MEMORY][2]; // 2D array with 1024 rows and 2 columns


// Function to read from memory at a specified address
int* mem_read(int addr, int pid) {
    // Check if the address is within valid bounds
    if (addr >= 0 && addr < MAX_MEMORY) {
        // Check if the read operation is allowed for the process
        if (is_allowed_address(pid, addr)) {
            // Return a pointer to the data at the specified address
            return memory[addr];
        }else {
            printf("\nMemory Access Violation\n");
        }

    }

    return NULL;

}

// Function to write data to memory at a specified address
void mem_write(int addr, int* data, int pid) {
    // Check if the address is within valid bounds
    if (addr >= 0 && addr < MAX_MEMORY) {
        // Check if the write operation is allowed for the process
        if (is_allowed_address(pid, addr)) {
            // Write the data to the specified address
            memory[addr][0] = data[0]; // OP Code
            memory[addr][1] = data[1]; // Argument
        }
    } 
    
}