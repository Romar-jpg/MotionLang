#include <stdio.h>
#include <stdlib.h>
#include "motionlang_parser.h"

// ============================================================================
// MANUAL STRING HELPERS (No <string.h> dependency)
// ============================================================================
static int str_length(const char *s) {
    int len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

static int str_equals(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) return 0;
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

static int starts_with(const char *str, const char *prefix) {
    int i = 0;
    while (prefix[i] != '\0') {
        if (str[i] == '\0' || str[i] != prefix[i]) return 0;
        i++;
    }
    return 1;
}

static int find_first_char(const char *str, char target) {
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == target) return i;
    }
    return -1;
}

static int find_last_char(const char *str, char target) {
    int last = -1;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == target) last = i;
    }
    return last;
}

static void copy_slice(char *dest, const char *src, int start, int len, int max_dest) {
    int count = (len < max_dest - 1) ? len : (max_dest - 1);
    for (int i = 0; i < count; i++) {
        dest[i] = src[start + i];
    }
    dest[count] = '\0';
}

static void trim_whitespace(char *str) {
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\r' || str[start] == '\n') {
        start++;
    }

    int end = str_length(str) - 1;
    while (end >= start && (str[end] == ' ' || str[end] == '\t' || str[end] == '\r' || str[end] == '\n')) {
        str[end--] = '\0';
    }

    if (start > 0) {
        int j = 0;
        for (int i = start; i <= end; i++) {
            str[j++] = str[i];
        }
        str[j] = '\0';
    }
}

// ============================================================================
// CLASSIFICATION REVERSE-MAPPER
// ============================================================================
static TokenType map_classification_to_type(const char *name) {
    if (str_equals(name, "DATA_TYPE_NUM"))           return TOKEN_NUM;
    if (str_equals(name, "DATA_TYPE_DEC"))           return TOKEN_DEC;
    if (str_equals(name, "DATA_TYPE_CHAR"))          return TOKEN_CHAR;
    if (str_equals(name, "DATA_TYPE_FLAG"))          return TOKEN_FLAG;
    if (str_equals(name, "DATA_TYPE_TEXT"))          return TOKEN_TEXT;
    if (str_equals(name, "DATA_TYPE_STASH"))         return TOKEN_STASH;

    if (str_equals(name, "STORAGE_HOLD"))            return TOKEN_HOLD;
    if (str_equals(name, "STORAGE_FIXED"))           return TOKEN_FIXED;

    if (str_equals(name, "NOISE_WORD"))              return TOKEN_NOISE;

    if (str_equals(name, "KEYWORD_IF"))              return TOKEN_IF;
    if (str_equals(name, "KEYWORD_ELSE"))            return TOKEN_ELSE;
    if (str_equals(name, "KEYWORD_ELSE_IF"))         return TOKEN_ELSE_IF;
    if (str_equals(name, "KEYWORD_SPIN"))            return TOKEN_SPIN;
    if (str_equals(name, "KEYWORD_HALT"))            return TOKEN_HALT;
    if (str_equals(name, "KEYWORD_SKIP"))            return TOKEN_SKIP;

    if (str_equals(name, "KEYWORD_GRAB"))            return TOKEN_GRAB;
    if (str_equals(name, "KEYWORD_EMIT"))            return TOKEN_EMIT;
    if (str_equals(name, "KEYWORD_PROMPT"))          return TOKEN_PROMPT;
    if (str_equals(name, "KEYWORD_ROUTINE"))         return TOKEN_ROUTINE;
    if (str_equals(name, "KEYWORD_RETURN"))          return TOKEN_RETURN;

    if (str_equals(name, "OPERATOR_DIV"))            return TOKEN_DIV_OP;
    if (str_equals(name, "LOGICAL_AND"))             return TOKEN_AND;
    if (str_equals(name, "LOGICAL_OR"))              return TOKEN_OR;
    if (str_equals(name, "LOGICAL_NOT"))             return TOKEN_NOT;

    if (str_equals(name, "IDENTIFIER"))              return TOKEN_IDENTIFIER;
    if (str_equals(name, "INTEGER_LITERAL"))         return TOKEN_INTEGER_LITERAL;
    if (str_equals(name, "DECIMAL_LITERAL"))         return TOKEN_DECIMAL_LITERAL;
    if (str_equals(name, "STRING_LITERAL"))          return TOKEN_STRING_LITERAL;
    if (str_equals(name, "CHAR_LITERAL"))            return TOKEN_CHAR_LITERAL;

    if (str_equals(name, "OPERATOR_PLUS"))           return TOKEN_PLUS;
    if (str_equals(name, "OPERATOR_MINUS"))          return TOKEN_MINUS;
    if (str_equals(name, "OPERATOR_MULTIPLY"))       return TOKEN_STAR;
    if (str_equals(name, "OPERATOR_DIVIDE"))         return TOKEN_SLASH;
    if (str_equals(name, "OPERATOR_MODULO"))         return TOKEN_MODULO;
    if (str_equals(name, "OPERATOR_EXPONENT"))       return TOKEN_CARET;
    if (str_equals(name, "ASSIGNMENT_OPERATOR"))     return TOKEN_ASSIGN;
    if (str_equals(name, "RELATIONAL_EQUAL"))        return TOKEN_EQUAL;
    if (str_equals(name, "RELATIONAL_NOTEQUAL"))     return TOKEN_NOT_EQUAL;
    if (str_equals(name, "RELATIONAL_LESS"))         return TOKEN_LESS;
    if (str_equals(name, "RELATIONAL_GREATER"))      return TOKEN_GREATER;
    if (str_equals(name, "RELATIONAL_LESSEQUAL"))    return TOKEN_LESS_EQUAL;
    if (str_equals(name, "RELATIONAL_GREATEREQUAL")) return TOKEN_GREATER_EQUAL;

    if (str_equals(name, "DELIM_LPAREN"))            return TOKEN_LPAREN;
    if (str_equals(name, "DELIM_RPAREN"))            return TOKEN_RPAREN;
    if (str_equals(name, "DELIM_LBRACKET"))          return TOKEN_LBRACKET;
    if (str_equals(name, "DELIM_RBRACKET"))          return TOKEN_RBRACKET;
    if (str_equals(name, "DELIM_LBRACE"))            return TOKEN_LBRACE;
    if (str_equals(name, "DELIM_RBRACE"))            return TOKEN_RBRACE;
    if (str_equals(name, "DELIM_COMMA"))             return TOKEN_COMMA;
    if (str_equals(name, "DELIM_SEMICOLON"))         return TOKEN_SEMICOLON;
    if (str_equals(name, "DELIM_SINGLE_QUOTE"))      return TOKEN_SINGLE_QUOTE;
    if (str_equals(name, "DELIM_DOUBLE_QUOTE"))      return TOKEN_DOUBLE_QUOTE;

    if (str_equals(name, "COMMENT"))                 return TOKEN_COMMENT;
    if (str_equals(name, "END_OF_FILE"))             return TOKEN_EOF;

    return TOKEN_UNKNOWN;
}

// ============================================================================
// 1. SYMBOL TABLE READER
// ============================================================================
int load_tokens_from_symbol_table(const char *filename, Parser *parser) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Parser Error: Could not open '%s'.\n", filename);
        return 0;
    }

    int capacity = 128;
    parser->tokens = (Token *)malloc((size_t)capacity * sizeof(Token));
    parser->count = 0;
    parser->current = 0;
    parser->error_count = 0;
    parser->panic_mode = 0;

    char line_buf[1024];
    while (fgets(line_buf, sizeof(line_buf), file)) {
        // Skip border dividers and header line
        if (starts_with(line_buf, "===") ||
            starts_with(line_buf, "TOKEN CLASSIFICATION")) {
            continue;
        }

        // Split row by '|' delimiters: [Classification] | [Lexeme] | [Coordinates]
        int first_pipe = find_first_char(line_buf, '|');
        if (first_pipe == -1) continue;

        int second_pipe = find_last_char(line_buf, '|');
        if (second_pipe == -1 || second_pipe == first_pipe) continue;

        // Extract classification column
        char class_col[64];
        copy_slice(class_col, line_buf, 0, first_pipe, sizeof(class_col));
        trim_whitespace(class_col);

        // Skip wrapped continuation lines where classification is empty
        if (class_col[0] == '\0') {
            continue;
        }

        // Extract lexeme column
        char lexeme_col[512];
        int lex_start = first_pipe + 1;
        int lex_len = second_pipe - lex_start;
        copy_slice(lexeme_col, line_buf, lex_start, lex_len, sizeof(lexeme_col));
        trim_whitespace(lexeme_col);

        // Extract line and column numbers from "Line: X Col: Y"
        int line_num = 0, col_num = 0;
        sscanf(line_buf + second_pipe + 1, " Line: %d Col: %d", &line_num, &col_num);

        // Expand dynamic array if needed
        if (parser->count >= capacity) {
            capacity *= 2;
            parser->tokens = (Token *)realloc(parser->tokens, (size_t)capacity * sizeof(Token));
        }

        TokenType type = map_classification_to_type(class_col);
        parser->tokens[parser->count] = createToken(type, lexeme_col, line_num, col_num);
        parser->count++;
    }

    fclose(file);
    // 1. Guard against empty files or passing a raw .mtnl file instead of Symbol Table.txt
    if (parser->count == 0) {
        fprintf(stderr, "Parser Error: No valid tokens loaded from '%s' (pass the Symbol Table file).\n", filename);
        free(parser->tokens);
        parser->tokens = NULL;
        return 0;
    }

    // 2. Ensure token stream always ends with TOKEN_EOF so advance() never stalls
    if (parser->tokens[parser->count - 1].type != TOKEN_EOF) {
        if (parser->count >= capacity) {
            capacity += 1;
            parser->tokens = (Token *)realloc(parser->tokens, (size_t)capacity * sizeof(Token));
        }
        Token last = parser->tokens[parser->count - 1];
        parser->tokens[parser->count] = createToken(TOKEN_EOF, "EOF", last.line, last.column);
        parser->count++;
    }
    // ===========================
    return 1;
}

void free_parser(Parser *parser) {
    if (parser && parser->tokens) {
        for (int i = 0; i < parser->count; i++) {
            freeToken(&parser->tokens[i]);
        }
        free(parser->tokens);
        parser->tokens = NULL;
    }
}

// ============================================================================
// 2. STREAM NAVIGATION UTILITIES
// ============================================================================
Token peek(Parser *parser) {
    if (parser->current >= parser->count) {
        return parser->tokens[parser->count - 1]; // TOKEN_EOF
    }
    return parser->tokens[parser->current];
}

Token peek_next(Parser *parser) {
    if (parser->current + 1 >= parser->count) {
        return parser->tokens[parser->count - 1];
    }
    return parser->tokens[parser->current + 1];
}

Token advance(Parser *parser) {
    if (parser->current < parser->count - 1) {
        parser->current++;
    }
    return parser->tokens[parser->current - 1];
}

int check(Parser *parser, TokenType type) {
    return peek(parser).type == type;
}

int match(Parser *parser, TokenType type) {
    if (check(parser, type)) {
        advance(parser);
        return 1;
    }
    return 0;
}

int consume(Parser *parser, TokenType type, const char *error_msg) {
    if (check(parser, type)) {
        advance(parser);
        return 1;
    }
    syntax_error(parser, error_msg);
    return 0;
}

// ============================================================================
// 3. ERROR DIAGNOSTICS & PANIC-MODE RECOVERY
// ============================================================================
void syntax_error(Parser *parser, const char *message) {
    if (parser->panic_mode) return;

    parser->panic_mode = 1;
    parser->error_count++;

    Token err_tok = peek(parser);
    fprintf(stderr, "[Syntax Error] Line %d, Col %d (near '%s'): %s\n",
            err_tok.line, err_tok.column, err_tok.lexeme, message);
}

void recover(Parser *parser) {
    parser->panic_mode = 0;

    // Always consume the offending token that triggered the error so we never loop infinitely
    if (!check(parser, TOKEN_EOF)) {
        advance(parser);
    }

    while (!check(parser, TOKEN_EOF)) {
        // 1. Synchronized if a semicolon or closing brace was just consumed
        if (parser->current > 0) {
            TokenType prev = parser->tokens[parser->current - 1].type;
            if (prev == TOKEN_SEMICOLON || prev == TOKEN_RBRACE) {
                return;
            }
        }

        // 2. Synchronize at statement terminator ';' or block end '}'
        if (check(parser, TOKEN_SEMICOLON)) {
            advance(parser); // Consume ';' so the next statement starts cleanly
            return;
        }
        if (check(parser, TOKEN_RBRACE)) {
            return; // Leave '}' for parse_block() to consume
        }

        // 3. Synchronize on the start of a new statement
        switch (peek(parser).type) {
            case TOKEN_HOLD:
            case TOKEN_FIXED:
            case TOKEN_IF:
            case TOKEN_SPIN:
            case TOKEN_GRAB:
            case TOKEN_EMIT:
            case TOKEN_ROUTINE:
            case TOKEN_RETURN:
            case TOKEN_HALT:
            case TOKEN_SKIP:
                return;
            default:
                advance(parser);
                break;
        }
    }
}

// ============================================================================
// 4. CORE GRAMMAR DISPATCHER (Recursive Descent)
// ============================================================================
void parse_program(Parser *parser) {
    parse_statement_list(parser);

    if (!check(parser, TOKEN_EOF)) {
        syntax_error(parser, "Unexpected tokens after end of program.");
    }
}

void parse_statement_list(Parser *parser) {
    while (!check(parser, TOKEN_EOF) && !check(parser, TOKEN_RBRACE)) {
        parse_statement(parser);

        if (parser->panic_mode) {
            recover(parser);
        }
    }
}

void parse_statement(Parser *parser) {
    Token current = peek(parser);

    switch (current.type) {
        case TOKEN_COMMENT:
            advance(parser);
            break;

        case TOKEN_HOLD:
        case TOKEN_FIXED:
            parse_declaration_stmt(parser);
            break;

        case TOKEN_IDENTIFIER:
            parse_assign_stmt(parser);
            break;

        case TOKEN_GRAB:
            parse_input_stmt(parser);
            break;

        case TOKEN_EMIT:
            parse_output_stmt(parser);
            break;

        case TOKEN_IF:
            parse_if_stmt(parser);
            break;

        case TOKEN_SPIN:
            parse_loop_stmt(parser);
            break;

        case TOKEN_HALT:
        case TOKEN_SKIP:
        case TOKEN_RETURN:
            parse_flow_stmt(parser);
            break;

        case TOKEN_ROUTINE:
            parse_routine_decl(parser);
            break;

        case TOKEN_UNKNOWN:
            syntax_error(parser, "Invalid lexical token encountered in source.");
            advance(parser);
            break;

        default:
            syntax_error(parser, "Expected a valid statement.");
            break;
    }
}

void parse_block(Parser *parser) {
    if (!consume(parser, TOKEN_LBRACE, "Expected '{' to start block.")) {
        return;
    }

    parse_statement_list(parser);

    consume(parser, TOKEN_RBRACE, "Expected '}' to close block.");
}