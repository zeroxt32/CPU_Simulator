#include <stdio.h>
#include "smm.h"

void print_allocation_table() {
    printf("Allocation Table:\n");
    printf("PID\tBase\tSize\n");
    for (int i = 0; i < 256; i++) {
        if (allocation_table[i][2] != 0) {  // Check if the row is not empty
            printf("%d\t%d\t%d\n", allocation_table[i][0], allocation_table[i][1], allocation_table[i][2]);
        }
    }
    printf("\n");
}

int main() {
    // Test Case 1: Allocate memory for a process
    int pid1 = 1;
    int size1 = 50;
    int result1 = allocate(pid1, size1);
    printf("Test Case 1: Allocate memory for PID %d (Result: %s)\n", pid1, result1 ? "Success" : "Failure");

    //printf("Print Allocation Table\n");
    print_allocation_table();
    // Test Case 2: Deallocate memory for a process
    deallocate(pid1);
    printf("Test Case 2: Deallocate memory for PID %d\n", pid1);

    printf("Print Allocation Table\n");
    print_allocation_table();
    print_holes_list();
    // Test Case 3: Attempt to allocate memory for a process with no available holes
    int pid2 = 2;
    int size2 = 100;
    int result2 = allocate(pid2, size2);
    printf("Test Case 3: Attempt to allocate memory for PID %d (Result: %s)\n", pid2, result2 ? "Success" : "Failure");

    // printf("Print Allocation Table\n");
    print_allocation_table();
    // Test Case 4: Allocate memory for another process after deallocation
    int pid3 = 3;
    int size3 = 30;
    int result3 = allocate(pid3, size3);
    printf("Test Case 4: Allocate memory for PID %d (Result: %s)\n", pid3, result3 ? "Success" : "Failure");
    printf("Print Allocation Table\n");
    print_allocation_table();
    print_holes_list();
    // Test Case 5: Check if an address is allowed for a process
    // int addr1 = 120;
    // int result4 = is_allowed_address(pid3, addr1);
    // printf("Test Case 5: Check if address %d is allowed for PID %d (Result: %s)\n", addr1, pid3, result4 ? "Allowed" : "Not Allowed");
    // printf("Print Allocation Table\n");
    // print_allocation_table();
    // Test Case 6: Check if an address is allowed for a process after deallocation
    //int addr2 = 25;
    //deallocate(pid3);
    //int result5 = is_allowed_address(pid3, addr2);
    // printf("Test Case 6: Check if address %d is allowed for PID %d after deallocation (Result: %s)\n", addr2, pid3, result5 ? "Allowed" : "Not Allowed");
    // printf("Print Allocation Table\n");
    // print_allocation_table();
    return 0;
}
