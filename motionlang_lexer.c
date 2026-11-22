#include <stdio.h>
#include <ctype.h>
#include "motionlang_tokens.h"
#include "stream.h" // Import Module 1

// ============================================================================
// MODULE 1 & 2 INTEGRATION BRIDGE
// ============================================================================
SourceStream *lexer_stream = NULL; // The global stream pointer

// Global coordinate trackers expected by Module 2
int current_line = 1;    
int current_column = 0; 

// Wrapper to fetch char and sync coordinates from Module 1
int getNextChar(void) {
    if (!lexer_stream) return EOF;
    int c = next_char(lexer_stream);
    current_line = lexer_stream->current_line;
    current_column = lexer_stream->current_column;
    return c;
}

// Wrapper to push a character back into Module 1's buffer
void ungetChar(int c) {
    if (c == EOF || !lexer_stream) return;
    
    // Utilize Module 1's peek buffer to "un-get" the character
    lexer_stream->peek_buffer = (char)c;
    lexer_stream->has_peeked = 1;
    
    // Reverse the column coordinate by 1 so it isn't double-counted
    lexer_stream->current_column--;
    current_column = lexer_stream->current_column;
}

// ============================================================================
// MODULE 3 IMPORTS (Token Structs & Keyword Registry)
// ============================================================================
extern Token createToken(TokenType type, const char* lexeme, int line, int column);

// ============================================================================
// MODULE 2 - PART A: Identifiers, Keywords and Reserved Words, Literals, Comments
// ============================================================================

static Token try_merge_else_if(int start_col) {
    int c1 = getNextChar();
    if (c1 != ' ' && c1 != '\t') {
        if (c1 != EOF) ungetChar(c1);
        return createToken(TOKEN_ELSE, "else", current_line, start_col);
    }

    int c2 = getNextChar();
    if (c2 != 'i') {
        if (c2 != EOF) ungetChar(c2);
        ungetChar(c1);
        return createToken(TOKEN_ELSE, "else", current_line, start_col);
    }

    int c3 = getNextChar();
    if (c3 != 'f') {
        if (c3 != EOF) ungetChar(c3);
        ungetChar(c2);
        ungetChar(c1);
        return createToken(TOKEN_ELSE, "else", current_line, start_col);
    }

    int c4 = getNextChar();
    if (c4 != EOF && (isalnum((unsigned char)c4) || c4 == '_')) {
        ungetChar(c4);
        ungetChar(c3);
        ungetChar(c2);
        ungetChar(c1);
        return createToken(TOKEN_ELSE, "else", current_line, start_col);
    }
    if (c4 != EOF) {
        ungetChar(c4);
    }

    return createToken(TOKEN_ELSE_IF, "else if", current_line, start_col);
}

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
    Token tok = createToken(TOKEN_IDENTIFIER, buffer, current_line, start_col);
    if (tok.type == TOKEN_ELSE) {
        freeToken(&tok);
        return try_merge_else_if(start_col);
    }
    return tok;
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
    int c;

    while ((c = getNextChar()) != EOF && c != quote_char) {
        if (c == '\n') {
            return createToken(TOKEN_UNKNOWN, "Unterminated string", current_line, start_col);
        }
        if (c == '\\') {
            int escaped = getNextChar();
            if (escaped == EOF) {
                return createToken(TOKEN_EOF, "Unexpected EOF in string", current_line, start_col);
            }
            if (escaped == '\n') {
                return createToken(TOKEN_UNKNOWN, "Unterminated string", current_line, start_col);
            }
            if (idx < (int)sizeof(buffer) - 2) {
                buffer[idx++] = '\\';
                buffer[idx++] = (char)escaped;
            }
            continue;
        }
        if (idx < (int)sizeof(buffer) - 1) {
            buffer[idx++] = (char)c;
        }
    }

    if (c == EOF) {
        return createToken(TOKEN_EOF, "Unexpected EOF in string", current_line, start_col);
    }

    buffer[idx] = '\0';

    if (quote_char == '\'') {
        if (idx == 0) {
            return createToken(TOKEN_UNKNOWN, "Empty character literal", current_line, start_col);
        }
        if (idx > 1 && !(idx == 2 && buffer[0] == '\\')) {
            return createToken(TOKEN_UNKNOWN, "Multi-character literal", current_line, start_col);
        }
    }
    
    return createToken(TOKEN_STRING_LITERAL, buffer, current_line, start_col);
}

// 4. Comments (# single-line and [# ... #] multi-line)
Token handle_comment(int is_multiline, int start_line, int start_col) {
    char buffer[512];
    int idx = 0;
    int c;

    if (is_multiline) {
        buffer[idx++] = '[';
        buffer[idx++] = '#';
        while ((c = getNextChar()) != EOF) {
            if (c == '#') {
                int next = getNextChar();
                if (next == ']') {
                    if (idx < (int)sizeof(buffer) - 2) {
                        buffer[idx++] = '#';
                        buffer[idx++] = ']';
                    }
                    buffer[idx] = '\0';
                    return createToken(TOKEN_COMMENT, buffer, start_line, start_col);
                }
                if (next != EOF) ungetChar(next);
            }

            // Flatten actual newlines so Symbol Table.txt stays on one row
            if (c == '\n') {
                if (idx < (int)sizeof(buffer) - 2) {
                    buffer[idx++] = '\\';
                    buffer[idx++] = 'n';
                }
            } else if (c != '\r') {
                if (idx < (int)sizeof(buffer) - 1) {
                    buffer[idx++] = (char)c;
                }
            }
        }
        buffer[idx] = '\0';
        return createToken(TOKEN_UNKNOWN, "Unterminated multi-line comment", start_line, start_col);
    } else {
        buffer[idx++] = '#';
        while ((c = getNextChar()) != EOF && c != '\n') {
            if (c != '\r' && idx < (int)sizeof(buffer) - 1) {
                buffer[idx++] = (char)c;
            }
        }
        buffer[idx] = '\0';
        return createToken(TOKEN_COMMENT, buffer, start_line, start_col);
    }
    // Fallback return so GCC knows all control paths return a Token
    return createToken(TOKEN_UNKNOWN, "Invalid comment", start_line, start_col);
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

        // Single-Line Comment Handling Dispatch
        case '#':
            return handle_comment(0, current_line, start_col);

        // Delimiters & Brackets
        case '(': 
            current_column++; 
            return createToken(TOKEN_LPAREN, "(", current_line, start_col);

        case ')': 
            current_column++; 
            return createToken(TOKEN_RPAREN, ")", current_line, start_col);

        // Left Bracket OR Multi-Line Comment ([# ... #]) Dispatch
        case '[': {
            int next = getNextChar();
            if (next == '#') {
                return handle_comment(1, current_line, start_col);
            }
            if (next != EOF) ungetChar(next);
            current_column++; 
            return createToken(TOKEN_LBRACKET, "[", current_line, start_col);
        }

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

        // Operations Symbols 
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
            if (next != EOF) ungetChar(next);
            current_column++;
            return createToken(TOKEN_ASSIGN, "=", current_line, start_col);
        }
        case '<': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_LESS_EQUAL, "<=", current_line, start_col);
            }
            if (next != EOF) ungetChar(next);
            current_column++;
            return createToken(TOKEN_LESS, "<", current_line, start_col);
        }
        case '>': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_GREATER_EQUAL, ">=", current_line, start_col);
            }
            if (next != EOF) ungetChar(next);
            current_column++;
            return createToken(TOKEN_GREATER, ">", current_line, start_col);
        }
        case '!': {
            int next = getNextChar();
            if (next == '=') {
                current_column += 2;
                return createToken(TOKEN_NOT_EQUAL, "!=", current_line, start_col);
            }
            if (next != EOF) ungetChar(next);
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