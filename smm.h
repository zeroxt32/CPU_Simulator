// smm.h

#ifndef SMM_H
#define SMM_H

// Structure to represent a hole in memory
typedef struct Hole {
    int base_address;
    int size;
    struct Hole* next;
} Hole;

// Structure to represent an entry in the allocation table
typedef struct AllocationEntry {
    int pid;
    int base_address;
    int size;
} AllocationEntry;

// Global variables
#define MAX_PROCESSES 256
#define MAX_MEMORY 1024

extern int hole_count;
extern AllocationEntry allocation_table[MAX_PROCESSES];


// Function prototypes
void init_smm();
int allocate(int pid, int size);
void deallocate(int pid);
void add_hole(int base, int size);
void remove_hole(int base);
void merge_holes();
int find_hole(int size);
int get_base_address(int pid);
int find_empty_row();
int is_allowed_address(int pid, int addr);
void print_holes_list();
void print_allocation_table();
#endif /* SMM_H */
