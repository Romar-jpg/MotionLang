#ifndef MOTIONLANG_TOKENS_H
#define MOTIONLANG_TOKENS_H

// Enumeration of all MotionLang token types
typedef enum {
      // Data Types
    TOKEN_NUM, TOKEN_DEC, TOKEN_CHAR, TOKEN_FLAG, TOKEN_TEXT, TOKEN_STASH,
    
    // Storage Constraints
    TOKEN_HOLD, TOKEN_FIXED,
    
    // Conditionals & Iteration
    TOKEN_IF, TOKEN_ELSE, TOKEN_SPIN, TOKEN_HALT, TOKEN_SKIP,
    
    // Input/Output & Functions
    TOKEN_GRAB, TOKEN_EMIT, TOKEN_PROMPT, TOKEN_ROUTINE, TOKEN_RETURN,
    
    // Word Operators
    TOKEN_DIV_OP, TOKEN_AND, TOKEN_OR, TOKEN_NOT,
    
    // Identifiers & Literals
    TOKEN_IDENTIFIER, 
    TOKEN_INTEGER_LITERAL, TOKEN_DECIMAL_LITERAL,
    TOKEN_STRING_LITERAL, TOKEN_CHAR_LITERAL,
    
    // Arithmetic & Relational Symbols
    TOKEN_PLUS, TOKEN_MINUS, TOKEN_STAR, TOKEN_SLASH, TOKEN_MODULO, TOKEN_CARET,
    TOKEN_ASSIGN, TOKEN_EQUAL, TOKEN_NOT_EQUAL, TOKEN_LESS, TOKEN_GREATER,
    TOKEN_LESS_EQUAL, TOKEN_GREATER_EQUAL,
    
    // Delimiters & Brackets
    TOKEN_LPAREN, TOKEN_RPAREN,    // ()
    TOKEN_LBRACKET, TOKEN_RBRACKET, // []
    TOKEN_LBRACE, TOKEN_RBRACE,     // {}
    TOKEN_COMMA, TOKEN_SEMICOLON,   // , ;
    
    // End of File / Unknown
    TOKEN_UNKNOWN, TOKEN_EOF
} TokenType;

// The primary Token structure
typedef struct {
    TokenType type;
    char* lexeme;
    int line;
    int column;
} Token;

// Function prototypes for Module 3
TokenType lookupKeyword(const char* lexeme);
Token createToken(TokenType type, const char* lexeme, int line, int column);
void freeToken(Token* token);
void printToken(const Token* token); // Useful for debugging

#endif