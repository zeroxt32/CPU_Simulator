#ifndef DISK_H
#define DISK_H

// Function prototypes for disk operations
void load_programs(char* fname);
int* translate(char* instruction);
int get_next_pid(int status);

#endif /* DISK_H */

