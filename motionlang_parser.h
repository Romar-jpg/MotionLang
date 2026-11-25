#ifndef MOTIONLANG_PARSER_H
#define MOTIONLANG_PARSER_H

#include <stdio.h>
#include "motionlang_tokens.h"

// Shared Parser State passed across all parser modules
typedef struct {
    Token *tokens;       // Dynamic array of tokens loaded from Symbol Table.txt
    int count;           // Total number of tokens loaded
    int current;         // Current index in the token stream
    int error_count;     // Total syntax errors encountered
    int panic_mode;      // Flag to suppress cascading errors during recovery
} Parser;

// ============================================================================
// 1. SYMBOL TABLE READER & CORE ENGINE 
// ============================================================================
int load_tokens_from_symbol_table(const char *filename, Parser *parser);
void free_parser(Parser *parser);

// Stream navigation & token matching utilities
Token peek(Parser *parser);
Token peek_next(Parser *parser);
Token advance(Parser *parser);
int check(Parser *parser, TokenType type);
int match(Parser *parser, TokenType type);
int consume(Parser *parser, TokenType type, const char *error_msg);

// Error Diagnostics & Panic-Mode Recovery
void syntax_error(Parser *parser, const char *message);
void recover(Parser *parser);

// Top-level Grammar Entry & Dispatcher
void parse_program(Parser *parser);
void parse_statement_list(Parser *parser);
void parse_statement(Parser *parser);
void parse_block(Parser *parser);

// ============================================================================
// 2. STATEMENT MODULES 
// ============================================================================
void parse_declaration_stmt(Parser *parser);
void parse_assign_stmt(Parser *parser);
void parse_input_stmt(Parser *parser);
void parse_output_stmt(Parser *parser);
void parse_if_stmt(Parser *parser);
void parse_loop_stmt(Parser *parser);
void parse_flow_stmt(Parser *parser);
void parse_routine_decl(Parser *parser);
void accept_noise(Parser *parser, const char *word);
void noise_misplaced(Parser *parser);

// ============================================================================
// 3. EXPRESSION MODULES 
// ============================================================================
void parse_expr(Parser *parser);
void parse_or_expr(Parser *parser);
void parse_and_expr(Parser *parser);
void parse_rel_expr(Parser *parser);
void parse_add_expr(Parser *parser);
void parse_mul_expr(Parser *parser);
void parse_unary(Parser *parser);
void parse_exponent_expr(Parser *parser);
void parse_primary(Parser *parser);

#endif