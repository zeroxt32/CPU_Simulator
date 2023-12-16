#include <stdio.h>
#include <stdlib.h>
#include "smm.h"
#include "scheduler.h"
// Global variables
int hole_count = 0;
Hole* holes = NULL;
AllocationEntry allocation_table[MAX_PROCESSES];
// Function to print the allocation table
void print_allocation_table() {
    printf("\nAllocation Table:\n");
    printf("PID\tBase Address\tSize\n");
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (allocation_table[i].size > 0) {
            printf("%d\t%d\t\t%d\n", allocation_table[i].pid, allocation_table[i].base_address, allocation_table[i].size);
        }
    }
}
// Function to print the holes linked list
void print_holes_list() {
    printf("\nHoles List:\n");
    Hole* current = holes;
    while (current != NULL) {
        printf("Base Address: %d, Size: %d\n", current->base_address, current->size);
        current = current->next;
    }
}

void init_smm(){
    holes = (Hole*)malloc(sizeof(Hole));
    holes->base_address = 0;
    holes->size = MAX_MEMORY;
    holes->next = NULL;
    hole_count++;
}
// Function to add a hole to the linked list
void add_hole(int base, int size) {
    Hole* new_hole = (Hole*)malloc(sizeof(Hole));
    new_hole->base_address = base + size;  // Update the base address
    new_hole->size = size;
    new_hole->next = NULL;
    Hole* current = holes;
    Hole* prev = NULL;
    // Iterate through the linked list to find the correct position to insert the new hole
    while (current != NULL && current->base_address < new_hole->base_address) {
        prev = current;
        current = current->next;
    }
    // Insert the new hole into the linked list
    if (prev != NULL) {
        prev->next = new_hole;
    } else {
        holes = new_hole;
    }
    new_hole->next = current;
    hole_count++;  // Increment hole_count when a new hole is added
    // Merge adjacent holes
    current = holes;
    while (current != NULL && current->next != NULL) {
        if ((current->base_address + current->size) == current->next->base_address) {
            current->size += current->next->size;
            Hole* temp = current->next;
            current->next = current->next->next;
            free(temp);
            hole_count--;  // Decrement hole_count when two holes are merged
        } else {
            current = current->next;
        }
    }
}
// Function to remove a hole from the linked list
void remove_hole(int base) {
    Hole* current = holes;
    Hole* prev = NULL;
    while (current != NULL) {
        if (current->base_address == base) {
            // Remove the hole from the linked list
            if (prev != NULL) {
                prev->next = current->next;
            } else {
                holes = current->next;
            }
            free(current);
            hole_count--;  // Decrement hole_count when a hole is removed
            break;
        }
        prev = current;
        current = current->next;
    }
    // Merge adjacent holes
    current = holes;
    while (current != NULL && current->next != NULL) {
        if ((current->base_address + current->size) == current->next->base_address) {
            current->size += current->next->size;
            Hole* temp = current->next;
            current->next = current->next->next;
            free(temp);
            // Don't decrement hole_count here
        } else {
            current = current->next;
        }
    }
}
// Function to find a hole using the First-Fit algorithm
int find_hole(int size) {
    Hole* current = holes;
    Hole* prev = NULL;
    while (current != NULL) {
        if (current->size >= size) {
            // Found a hole
            int base_address = current->base_address;
            // Update the hole
            current->base_address += size;
            current->size -= size;
            // If the entire hole is used, remove it from the linked list
            if (current->size == 0) {
                if (prev != NULL) {
                    prev->next = current->next;
                } else {
                    holes = current->next;
                }
                free(current);
                hole_count--;
            }
            // Add the hole to the allocation table
            add_hole(base_address+size, size);
            return base_address;
        }
        prev = current;
        current = current->next;
    }
    // No suitable hole found
    return -1;
}
// Function to allocate memory for a process
int allocate(int pid, int size) {
    int base_address = find_hole(size);
    if (base_address == -1) {
        // No suitable hole found
        printf("\nError: Insufficient memory to allocate for Process %d.\n", pid);
        return 0;
    }
    // Allocate memory for the process
    allocation_table[pid].pid = pid;
    allocation_table[pid].base_address = base_address;
    allocation_table[pid].size = size;
    printf("\nProcess PID =  %d allocated successfully. Base Address: %d\n\n", pid, base_address);


    return 1;
}
// Function to deallocate memory when a process is terminated
void deallocate(int pid) {
    // Add a new hole to the linked list
    add_hole(allocation_table[pid].base_address, allocation_table[pid].size);
    // Update the allocation table
    allocation_table[pid].size = 0;
    printf("\n\nProcess %d deallocated successfully.\n\n", pid);

}
// Function to get the base address of a process
int get_base_address(int pid) {
    return allocation_table[pid].base_address;
}

// Function to find an empty row in the allocation table
int find_empty_row() {
    for (int i = 0; i < MAX_PROCESSES; i++) {
        if (allocation_table[i].size == 0) {
            return i;
        }
    }
    return -1;
}
// Function to check if a memory operation is allowed
int is_allowed_address(int pid, int addr) {
    if (allocation_table[pid].size > 0) {
        int start_address = allocation_table[pid].base_address;
        int end_address = start_address + allocation_table[pid].size - 1;
        if (addr >= start_address && addr <= end_address) {
            return 1; // Allowed
        }
    }
    return 0; // Not Allowed
}