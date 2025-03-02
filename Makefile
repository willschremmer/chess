# Compiler and flags
CC = gcc
CFLAGS = -I/usr/local/include         # Include directory for Raylib headers
LDFLAGS = -L/usr/local/lib            # Library directory for Raylib
LIBS = -lraylib -lm                   # Link against Raylib library

# Source files and target executable
SRC = main.c board.c                  # Replace with your source file(s)
OBJ = $(SRC:.c=.o)                    # Object files (compiled .c files)
EXEC = chess                          # The name of the final executable

# Default target to build the program
$(EXEC): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS) $(LIBS)  # Link the object files to create the executable

# Rule to compile .c files into .o files
%.o: %.c
	$(CC) -c $< $(CFLAGS)  # Compile the .c file into an object file

# Clean up the generated files
clean:
	rm -f $(OBJ) $(EXEC)  # Delete object files and the executable
