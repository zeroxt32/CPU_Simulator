#ifndef CPU_H
#define CPU_H

// Define CPU registers
extern int Base;
extern int PC;
extern int IR0;
extern int IR1;
extern int AC;
extern int MAR;
extern int MBR;

// Define the register_struct for context switching
typedef struct {
    int base;
    int pc;
    int ir0;
    int ir1;
    int ac;
    int mar;
    int mbr;
} register_struct;

// Function prototypes for CPU operations
void fetch_instruction(int addr);
void execute_instruction();
int mem_address(int l_addr);
int clock_cycle();

// Function for context switching
register_struct context_switch(register_struct new_vals);

#endif /* CPU_H */

