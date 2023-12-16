# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -lm

# Executable name
EXEC = smm_scheduler_simulator

# Source files
SRCS = cpu.c memory.c disk.c scheduler.c smm.c main.c

# Object files
OBJS = $(SRCS:.c=.o)

# Header files
HDRS = cpu.h memory.h disk.h scheduler.h smm.h

# Dependency files
DEPS = $(HDRS)

# Default target
all: $(EXEC)

# Build executable
$(EXEC): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(EXEC)

# Compile source files
%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJS) $(EXEC)

.PHONY: all clean

