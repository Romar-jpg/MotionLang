#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "motionlang_tokens.h"

// Keyword registry: Identifies if a string is a reserved word[cite: 5, 6]
TokenType lookupKeyword(const char* lexeme) {
    if (strcmp(lexeme, "num") == 0) return TOKEN_NUM;
    if (strcmp(lexeme, "dec") == 0) return TOKEN_DEC;
    if (strcmp(lexeme, "char") == 0) return TOKEN_CHAR;
    if (strcmp(lexeme, "flag") == 0) return TOKEN_FLAG;
    if (strcmp(lexeme, "text") == 0) return TOKEN_TEXT;
    if (strcmp(lexeme, "stash") == 0) return TOKEN_STASH;
    
    if (strcmp(lexeme, "hold") == 0) return TOKEN_HOLD;
    if (strcmp(lexeme, "fixed") == 0) return TOKEN_FIXED;
    
    if (strcmp(lexeme, "if") == 0) return TOKEN_IF;
    if (strcmp(lexeme, "else") == 0) return TOKEN_ELSE;
    
    if (strcmp(lexeme, "spin") == 0) return TOKEN_SPIN;
    if (strcmp(lexeme, "halt") == 0) return TOKEN_HALT;
    if (strcmp(lexeme, "skip") == 0) return TOKEN_SKIP;
    
    if (strcmp(lexeme, "grab") == 0) return TOKEN_GRAB;
    if (strcmp(lexeme, "emit") == 0) return TOKEN_EMIT;
    if (strcmp(lexeme, "prompt") == 0) return TOKEN_PROMPT;
    
    if (strcmp(lexeme, "routine") == 0) return TOKEN_ROUTINE;
    if (strcmp(lexeme, "return") == 0) return TOKEN_RETURN;
    
    if (strcmp(lexeme, "div") == 0) return TOKEN_DIV_OP;
    if (strcmp(lexeme, "AND") == 0) return TOKEN_AND;
    if (strcmp(lexeme, "OR") == 0) return TOKEN_OR;
    if (strcmp(lexeme, "NOT") == 0) return TOKEN_NOT;

    return TOKEN_IDENTIFIER;
}

// Packages the lexeme and coordinates into a Token struct[cite: 11]
Token createToken(TokenType type, const char* lexeme, int line, int column) {
    Token newToken;
    
    // Route alphanumeric strings through the registry to check for keywords
    if (type == TOKEN_IDENTIFIER) {
        newToken.type = lookupKeyword(lexeme);
    } else {
        newToken.type = type;
    }
    
    newToken.lexeme = strdup(lexeme); 
    newToken.line = line;
    newToken.column = column;
    
    return newToken;
}

void freeToken(Token* token) {
    if (token->lexeme != NULL) {
        free(token->lexeme);
        token->lexeme = NULL;
    }
}

// Helper to print tokens to the terminal for Module 4 test suite debugging
void printToken(const Token* token) {
    printf("Token [Type: %2d, Line: %3d, Col: %3d] -> %s\n", 
           token->type, token->line, token->column, token->lexeme);
}