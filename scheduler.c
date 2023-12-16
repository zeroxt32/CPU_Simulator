#include <stdio.h>
#include <stdlib.h>
#include "scheduler.h"
#include "cpu.h"

// Define the ready queue as a linked list
typedef struct Node {
    PCB process;
    struct Node* next;
} Node;

Node* ready_queue = NULL; // Initialize the ready queue as empty
int time_quantum = 10;    // Set the time quantum to 10

// Initialize the process table
PCB process_table[MAX_PROCESS_COUNT];

// Initialize the process ID counter
int process_id_counter = 0;

// Function to initialize the scheduler
void init_scheduler() {
    // Initialize the ready queue as empty
    ready_queue = NULL;
    
    // Initialize the process table
    for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
        process_table[i].process_id = -1; // -1 indicates an empty slot
        process_table[i].process_size = 0;
        process_table[i].registers.ac = 0;
        process_table[i].registers.base = 0;
        process_table[i].registers.ir0 = 0;
        process_table[i].registers.ir1 = 0;
        process_table[i].registers.mar = 0;
        process_table[i].registers.mbr = 0;
        process_table[i].registers.pc = 0;
        
        
    }
    
    // Reset the process ID counter
    process_id_counter = 0;
}

// Function to add a new process to the ready queue
int new_process(int pid, int base, int size) {
    // Check if the process table is full
    if (process_id_counter >= MAX_PROCESS_COUNT) {
        printf("\nError: Process table is full. Cannot create a new process.\n");
        return -1;
    }
    
    // Find an empty slot in the process table
    int slot = -1;
    for (int i = 0; i < MAX_PROCESS_COUNT; i++) {
        if (process_table[i].process_id == -1) {
            slot = i;
            break;
        }
    }
    
    // Check if an empty slot was found
    if (slot != -1) {
        // Create a new process in the process table
        process_table[slot].process_id = pid;//process_id_counter++;
        process_table[slot].process_size = size;
        process_table[slot].registers.base = base;
        
        // Add the new process to the ready queue
        Node* new_node = (Node*)malloc(sizeof(Node));
        if (new_node == NULL) {
            perror("\nNew Process Memory Allocation failure\n");
            exit(EXIT_FAILURE);
        }
        new_node->process = process_table[slot];
        new_node->next = NULL;
        
        // Add the new process to the end of the ready queue
        if (ready_queue == NULL) {
            ready_queue = new_node;
            //if the queue is empty we need to load the first address of the instruction of the first program inside the cpu registers
            context_switch(ready_queue->process.registers);
        } else {
            //printf("While loop\n\n");
            Node* current = ready_queue;
            if (current->next == NULL){
                //printf("Current-> null\n\n");
                current->next = new_node;
            }
            else{
                while (current->next != NULL) {
                    current = current->next;
                    //printf("Found last process\n\n");
                }
                current->next = new_node;
            }
        }
        
        printf("\n New process created: PID = %d, Size=%d , Base = %d\n", process_table[slot].process_id, size, base);
        return process_table[slot].process_id;
    } else {
        printf("\n Error: No empty slot available in the process table.\n");
        return -1;
    }
    
}

// Function to perform scheduling and context switching
int schedule(int cycle_num, int process_status) {

    static int current_time_quantum = 0;
    if (ready_queue == NULL){
        return 0;
    }
    if (process_status == 0){
        printf("\nProcess with PID = %d has terminated\n", ready_queue->process.process_id);

        //remove current_process from queue
        Node *temp = ready_queue;
        if (temp->next != NULL){
            ready_queue = ready_queue->next;
            context_switch(ready_queue->process.registers);
            free(temp);
            current_time_quantum = 0;
            return 1;
        }
        //if last process end program return 0
        else if (temp->next == NULL){
            free(temp);
            return 0;
        }
    }


    if (current_time_quantum == time_quantum){
        
        //return execution if this is last node in queue
        if (ready_queue->next == NULL){
            printf("\nTime quantum expired for process with last PID=%d.\n", get_current_process_pid());//ready_queue->process.process_id);
            printf("\n");
            current_time_quantum = 0;
            return 1;
        }
        
        Node *first = ready_queue;
        ready_queue = ready_queue->next;
        first->process.registers = context_switch(ready_queue->process.registers);

        Node *current = ready_queue;
        while(current->next != NULL){
            
            current = current->next;
        }
        first->next = NULL;

        current->next = first;

        current_time_quantum = 0;
        printf("\nTime quantum expired for process with PID=%d.\n", first->process.process_id);//ready_queue->process.process_id);
        printf("Clock Cycle %d - Executing Process with PID=%d\n", cycle_num, ready_queue->process.process_id);
        return 1;
    }
    printf("Clock Cycle %d - Executing Process with PID=%d\n", cycle_num, ready_queue->process.process_id);
    current_time_quantum++;
    return 1;
}

// Function to remove a process from the ready queue
void remove_process(int pid) {
    Node *current = ready_queue;
    Node *prev = NULL;
    if (current->next == NULL){
        
        free(current);
        ready_queue = NULL;
        return;
        
    }
    while (current != NULL) {
        if (current->process.process_id == pid) {
            // Remove the process from the ready queue
            if (prev != NULL) {
                prev->next = current->next;
                context_switch(prev->next->process.registers);
            } else {
                ready_queue = current->next;
                context_switch(ready_queue->process.registers);
            }
            free(current);
            break;
        }

        prev = current;
        current = current->next;
    }
    

}

// Function to get the PID of the currently running process
int get_current_process_pid() {
    if (ready_queue != NULL) {
        return ready_queue->process.process_id;
    } else {
        return -1;  // No currently running process
    }
}
