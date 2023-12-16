#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "memory.h"
#include "scheduler.h"
#include "smm.h"
// Define the maximum length of an instruction line
#define MAX_LINE_LENGTH 100

// Function to load and translate a program
int load_prog(char* fname, int addr) {
    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        
        perror("Error opening program file");
        //printf("%s", fname);
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    int instruction_counter = addr;

    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline character at the end of the line
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
        }

        // Translate the instruction and write it to memory
        int* translated = translate(line);
        if (translated != NULL) {
            memory[instruction_counter][0] = translated[0]; // OP Code
            memory[instruction_counter][1] = translated[1]; // Argument
            instruction_counter++;
        }

        free(translated);
    }

    fclose(file);
    return instruction_counter;
}

// Function to translate an instruction into its associated integer OP code
int* translate(char* instruction) {
    int* op_code_and_arg = (int*)malloc(2 * sizeof(int));

    if (op_code_and_arg == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    if(strncmp(instruction, "exit", 5) == 0){
        op_code_and_arg[0] = 0;
        op_code_and_arg[1] = 0;
    }else if (strncmp(instruction, "load_const", 10) == 0) {
        op_code_and_arg[0] = 1; // OP Code for load_const
        op_code_and_arg[1] = atoi(instruction + 10); // Argument
    } else if (strcmp(instruction, "move_from_mbr") == 0) {
        op_code_and_arg[0] = 2; // OP Code for move_from_mbr
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "move_from_mar", 13) == 0){
        op_code_and_arg[0] = 3; // OP Code for move_from_mar
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "move_to_mbr", 11) == 0){
        op_code_and_arg[0] = 4; // OP Code for move_to_mbr
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "move_to_mar", 11) == 0){
        op_code_and_arg[0] = 5; // OP Code for move_to_mar
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "load_at_addr", 12) == 0){
        op_code_and_arg[0] = 6; // OP Code for load_at_addr
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "write_at_addr", 13) == 0){
        op_code_and_arg[0] = 7; // OP Code for write_at_addr
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "add", 3) == 0){
        op_code_and_arg[0] = 8; // OP Code for add
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "multiply", 8) == 0){
        op_code_and_arg[0] = 9; // OP Code for multiply
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "and", 3) == 0){
        op_code_and_arg[0] = 10; // OP Code for and
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "or", 2) == 0){
        op_code_and_arg[0] = 11; // OP Code for or
        op_code_and_arg[1] = 0; // No argument
    }else if(strncmp(instruction, "ifgo", 4) == 0){
        op_code_and_arg[0] = 12; // OP Code for ifgo
        op_code_and_arg[1] = atoi(instruction + 4); //ifgo argument
        
    }else if(strncmp(instruction, "sleep", 5) == 0){
        op_code_and_arg[0] = 13; // OP Code for sleep
        op_code_and_arg[1] = 0; // No argument
    } 

    else {
        printf("Comment: %s\n", instruction);
        free(op_code_and_arg);
        return NULL;
    }

    return op_code_and_arg;
}

// Function to load programs from a text file into memory
void load_programs(char* fname) {


    FILE* file = fopen(fname, "r");
    if (file == NULL) {
        //printf("%s\n" , fname);
        perror("Error opening program list file");
        exit(EXIT_FAILURE);
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file) != NULL) {
        int size;
        char program_filename[MAX_LINE_LENGTH];
        if (sscanf(line, "%d %s", &size, program_filename) == 2) {
            // Load the program from the specified filename into memory at the given position
            //call smm
            int pid_status = 1;
            int pid = get_next_pid(pid_status);
            int allocation_result = allocate(pid, size);

            if(allocation_result){ //we have memory for program to be read into memory and new process created
                //printf("filename %s /n", program_filename );
                load_prog(program_filename, get_base_address(pid));
                int result = new_process(pid, get_base_address(pid),  size);
                if (result == -1){
                    get_next_pid(0);//reset the pid 
                    deallocate(pid);

                }
            }
            else{
                pid_status = 0;
                get_next_pid(pid_status);//reset the counter
                printf("\nError Allocating Memory Holes\n");
            }

        } else {
            printf("Invalid program list format: %s\n", line);
        }
    }
    fclose(file);
    
}

// Function to get the next available PID
int get_next_pid(int pid_status) {
    static int next_pid = 0;
    if(pid_status == 1){
        return next_pid++;
    }
    else{
        return next_pid--;
    }
}
