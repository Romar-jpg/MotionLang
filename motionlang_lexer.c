#include <stdio.h>
#include <ctype.h>
#include "motionlang_tokens.h"

// ============================================================================
// MODULE 1 IMPORTS (Engine & Stream Handlers)
// ============================================================================
extern int getNextChar(void);
extern void ungetChar(int c);
extern int current_line;    
extern int current_column; 
// might change if may Module 1 na

// ============================================================================
// MODULE 3 IMPORTS (Token Structs & Keyword Registry)
// ============================================================================
// Module 3's DFA automatically upgrades TOKEN_IDENTIFIER to keywords
extern Token createToken(TokenType type, const char* lexeme, int line, int column);
// ============================================================================
// MODULE 2 - PART A: Identfiers, Keywords and Reserved Words, Literals, Comments
// ============================================================================

// 1. Identifiers (Module 3 automatically upgrades to keywords)
Token handle_identifier(int first_char) {
    char buffer[100];
    int idx = 0;
    int start_col = current_column;

    if (!isalpha((unsigned char)first_char)) {
        return createToken(TOKEN_UNKNOWN, "Invalid identifier", current_line, start_col);
    }

    buffer[idx++] = (char)first_char;

    int c;
    while ((c = getNextChar()) != EOF) {
        if (isalpha((unsigned char)c) || c == '_') {
            if (idx < (int)sizeof(buffer) - 1) {
                buffer[idx++] = (char)c;
            }
        } else {
            ungetChar(c);
            break;
        }
    }
    buffer[idx] = '\0';

    // Module 3 internally checks if this buffer matches a reserved word
    return createToken(TOKEN_IDENTIFIER, buffer, current_line, start_col);
}

// 2. Number Literals (Integers & Decimals)
Token handle_number(int first_char) {
    char buffer[100];
    int idx = 0;
    int start_col = current_column;
    buffer[idx++] = (char)first_char;

    TokenType type = TOKEN_INTEGER_LITERAL;
    int state = 0; // 0: integer digits, 1: saw decimal dot, 2: fraction digits
    int c;

    while ((c = getNextChar()) != EOF) {
        switch (state) {
            case 0:
                if (isdigit(c)) {
                    if (idx < (int)sizeof(buffer) - 1) buffer[idx++] = (char)c;
                } else if (c == '.') {
                    state = 1;
                    if (idx < (int)sizeof(buffer) - 1) buffer[idx++] = (char)c;
                } else {
                    ungetChar(c);
                    buffer[idx] = '\0';
                    return createToken(type, buffer, current_line, start_col);
                }
                break;

            case 1:
                if (isdigit(c)) {
                    state = 2;
                    type = TOKEN_DECIMAL_LITERAL;
                    if (idx < (int)sizeof(buffer) - 1) buffer[idx++] = (char)c;
                } else {
                    ungetChar(c);
                    buffer[idx] = '\0';
                    return createToken(TOKEN_UNKNOWN, buffer, current_line, start_col);
                }
                break;

            case 2:
                if (isdigit(c)) {
                    if (idx < (int)sizeof(buffer) - 1) buffer[idx++] = (char)c;
                } else {
                    ungetChar(c);
                    buffer[idx] = '\0';
                    return createToken(type, buffer, current_line, start_col);
                }
                break;
        }
    }

    buffer[idx] = '\0';
    return createToken(type, buffer, current_line, start_col);
}

// 3. Text Literals
Token handle_string(int quote_char) {
    char buffer[100];
    int idx = 0;
    int start_col = current_column;
    int valid = 1;
    int c;

    while ((c = getNextChar()) != EOF && c != quote_char) {
        if (c == '\n') {
            return createToken(TOKEN_UNKNOWN, "Unterminated string", current_line, start_col);
        }
        if (!isalnum((unsigned char)c)) {
            valid = 0;
        }
        if (idx < (int)sizeof(buffer) - 1) {
            buffer[idx++] = (char)c;
        }
    }

    if (c == EOF) {
        return createToken(TOKEN_EOF, "Unexpected EOF in string", current_line, start_col);
    }

    buffer[idx] = '\0';
    if (!valid) {
        return createToken(TOKEN_UNKNOWN, buffer, current_line, start_col);
    }
    return createToken(TOKEN_STRING_LITERAL, buffer, current_line, start_col);
}

// 4. Comments #
void skip_comment(void) {
    int c;

    c = getNextChar();
    if (c == '[') {
        while ((c = getNextChar()) != EOF) {
            if (c == '#') {
                c = getNextChar();
                if (c == ']') {
                    return;
                }
                if (c == EOF) {
                    return;
                }
                ungetChar(c);
            }
        }
        return;
    }

    if (c != EOF && c != '\n') {
        ungetChar(c);
    }
    while ((c = getNextChar()) != EOF && c != '\n') {
        // Discard single-line comment contents until line end.
    }
}


// =============================================================================================
//    MODULE 2  - PART B: Main Switch loop, Operations Symbol, Blanks, Delimiters, Fallbacks
// =============================================================================================

Token getNextToken(void) {
    int c = getNextChar();

    if (c == EOF) {
        return createToken(TOKEN_EOF, "EOF", current_line, current_column);
    }

    // Blanks 
    while (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
        if (c == '\n') {
            current_line++;
            current_column = 1;
        } else {
            current_column++;
        }
        c = getNextChar();
        
        if (c == EOF) {
            return createToken(TOKEN_EOF, "EOF", current_line, current_column); 
        }
    } 

int start_col = current_column;

    // Main Switch-Case
    switch (c) {
      
        // Identifiers & Keywords Dispatch
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
            return handle_identifier(c);

        // Number Literals Dispatch
        case '0' ... '9':
            return handle_number(c);

        // String Literals Dispatch
        case '"':
        case '\'':
            return handle_string(c);

        // Comment Handling Dispatch
        case '#':
            skip_comment();
            return getNextToken();

        // Delimiters & Brackets[cite: 5, 6]
        case '(': 
            current_column++; 
            return createToken(TOKEN_LPAREN, "(", current_line, start_col);

        case ')': 
            current_column++; 
            return createToken(TOKEN_RPAREN, ")", current_line, start_col);

        case '[': 
            current_column++; 
            return createToken(TOKEN_LBRACKET, "[", current_line, start_col);

        case ']': 
            current_column++; 
            return createToken(TOKEN_RBRACKET, "]", current_line, start_col);

        case '{': 
            current_column++; 
            return createToken(TOKEN_LBRACE, "{", current_line, start_col);

        case '}': 
            current_column++; 
            return createToken(TOKEN_RBRACE, "}", current_line, start_col);

        case ',': 
            current_column++; 
            return createToken(TOKEN_COMMA, ",", current_line, start_col);

        case ';': 
            current_column++; 
            return createToken(TOKEN_SEMICOLON, ";", current_line, start_col);

        // Operations Symbols [cite: 7]
        // Arithmetic Operators
        case '+': 
            current_column++; 
            return createToken(TOKEN_PLUS, "+", current_line, start_col);

        case '-': 
            current_column++; 
            return createToken(TOKEN_MINUS, "-", current_line, start_col);

        case '*': 
            current_column++; 
            return createToken(TOKEN_STAR, "*", current_line, start_col);

        case '/': 
            current_column++; 
            return createToken(TOKEN_SLASH, "/", current_line, start_col);

        case '%': 
            current_column++; 
            return createToken(TOKEN_MODULO, "%", current_line, start_col);
            
        case '^': 
            current_column++; 
            return createToken(TOKEN_CARET, "^", current_line, start_col);

        // Relational Operators
        case '=': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_EQUAL, "==", current_line, start_col);
            }
            ungetChar(next);
            current_column++;
            return createToken(TOKEN_ASSIGN, "=", current_line, start_col);
        }
        case '<': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_LESS_EQUAL, "<=", current_line, start_col);
            }
            ungetChar(next);
            current_column++;
            return createToken(TOKEN_LESS, "<", current_line, start_col);
        }
        case '>': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_GREATER_EQUAL, ">=", current_line, start_col);
            }
            ungetChar(next);
            current_column++;
            return createToken(TOKEN_GREATER, ">", current_line, start_col);
        }
        case '!': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_NOT_EQUAL, "!=", current_line, start_col);
            }
            ungetChar(next);
            current_column++;
            return createToken(TOKEN_NOT, "!", current_line, start_col);
        }

        // Lexical Errors/Fallback
        // If a character is not recognized, an error will occur and it would return TOKEN_UNKNOWN
        default: {
            char lexeme[2] = { (char)c, '\0' };
            current_column++;
            fprintf(stderr, "Lexical Error: Unexpected character '%c' at line %d, column %d\n", c, current_line, start_col);
            return createToken(TOKEN_UNKNOWN, lexeme, current_line, start_col);
        }
    }
}