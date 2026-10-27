# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable name
TARGET = motionlang_lexer

# Source files
SRCS = main.c motionlang_tokens.c motionlang_lexer.c motionlang_stream.c

# Default rule to build the executable
all: $(TARGET)

# Rule to compile the source files into the target
$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) $(SRCS) -o $(TARGET)

# Rule to clean up generated files
clean:
	del /f /q $(TARGET).exe "Symbol Table.txt"