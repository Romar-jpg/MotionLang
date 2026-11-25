# Compiler and flags
CC = gcc
CFLAGS = -Wall -g

# Target executable names
LEXER_TARGET = motionlang_lexer
PARSER_TARGET = motionlang_parser

# Lexer source files
LEXER_SRCS = main.c motionlang_tokens.c motionlang_lexer.c motionlang_stream.c

# Parser source files (excludes main.c and parser_stubs.c to avoid duplicate definitions)
PARSER_SRCS = parser_main.c motionlang_tokens.c motionlang_parser.c \
              motionlang_parser_assign.c motionlang_parser_conditional.c \
              motionlang_parser_exprs.c motionlang_parser_io.c \
              motionlang_parser_iterative.c Motionlang_parser_noise.c

# Default rule builds both executables
all: $(LEXER_TARGET) $(PARSER_TARGET)

$(LEXER_TARGET): $(LEXER_SRCS)
	$(CC) $(CFLAGS) $(LEXER_SRCS) -o $(LEXER_TARGET)

$(PARSER_TARGET): $(PARSER_SRCS)
	$(CC) $(CFLAGS) $(PARSER_SRCS) -o $(PARSER_TARGET)

# Clean generated binaries and output
clean:
	del /f /q $(LEXER_TARGET).exe $(PARSER_TARGET).exe "Symbol Table.txt"