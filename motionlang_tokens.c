#include <stdio.h>
#include <stdlib.h>
#include "motionlang_tokens.h"

// DFA State Machine simulating keyword transitions[cite: 11]
TokenType lookupKeyword(const char* lexeme) {
    int state = 0; 
    int i = 0;

    while (lexeme[i] != '\0') {
        char c = lexeme[i];

        switch (state) {
            // q0: Start State - Evaluate the first character
            case 0:
                if (c == 's') state = 10;      // spin, skip, stash
                else if (c == 'h') state = 20; // hold, halt
                else if (c == 'd') state = 30; // dec, div
                else if (c == 'n') state = 40; // num
                else if (c == 'c') state = 50; // char
                else if (c == 'f') state = 60; // flag, fixed
                else if (c == 't') state = 70; // text
                else if (c == 'i') state = 80; // if
                else if (c == 'e') state = 90; // else, emit
                else if (c == 'g') state = 110; // grab
                else if (c == 'p') state = 120; // prompt
                else if (c == 'r') state = 130; // routine, return
                else if (c == 'A') state = 150; // AND
                else if (c == 'O') state = 160; // OR
                else if (c == 'N') state = 170; // NOT
                else return TOKEN_IDENTIFIER;  
                break;

            // ==========================================
            // DFA Branch: 's' (spin, skip, stash)
            // ==========================================
            case 10:
                if (c == 'p') state = 11;
                else if (c == 'k') state = 15;
                else if (c == 't') state = 18;
                else return TOKEN_IDENTIFIER;
                break;

            case 11: if (c == 'i') state = 12; else return TOKEN_IDENTIFIER; break;
            case 12: if (c == 'n') state = 13; else return TOKEN_IDENTIFIER; break; // spin[cite: 5]
            case 13: return TOKEN_IDENTIFIER; 

            case 15: if (c == 'i') state = 16; else return TOKEN_IDENTIFIER; break;
            case 16: if (c == 'p') state = 17; else return TOKEN_IDENTIFIER; break; // skip[cite: 6]
            case 17: return TOKEN_IDENTIFIER; 

            case 18: if (c == 'a') state = 19; else return TOKEN_IDENTIFIER; break;
            case 19: if (c == 's') state = 101; else return TOKEN_IDENTIFIER; break;
            case 101: if (c == 'h') state = 102; else return TOKEN_IDENTIFIER; break; // stash[cite: 5]
            case 102: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'h' (hold, halt)
            // ==========================================
            case 20:
                if (c == 'o') state = 21;
                else if (c == 'a') state = 25;
                else return TOKEN_IDENTIFIER;
                break;

            case 21: if (c == 'l') state = 22; else return TOKEN_IDENTIFIER; break;
            case 22: if (c == 'd') state = 23; else return TOKEN_IDENTIFIER; break; // hold[cite: 5]
            case 23: return TOKEN_IDENTIFIER;

            case 25: if (c == 'l') state = 26; else return TOKEN_IDENTIFIER; break;
            case 26: if (c == 't') state = 27; else return TOKEN_IDENTIFIER; break; // halt[cite: 6]
            case 27: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'd' (dec, div)
            // ==========================================
            case 30:
                if (c == 'e') state = 31;
                else if (c == 'i') state = 35;
                else return TOKEN_IDENTIFIER;
                break;

            case 31: if (c == 'c') state = 32; else return TOKEN_IDENTIFIER; break; // dec[cite: 5]
            case 32: return TOKEN_IDENTIFIER;

            case 35: if (c == 'v') state = 36; else return TOKEN_IDENTIFIER; break; // div[cite: 6]
            case 36: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'n' (num)
            // ==========================================
            case 40: if (c == 'u') state = 41; else return TOKEN_IDENTIFIER; break;
            case 41: if (c == 'm') state = 42; else return TOKEN_IDENTIFIER; break; // num[cite: 5]
            case 42: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'c' (char)
            // ==========================================
            case 50: if (c == 'h') state = 51; else return TOKEN_IDENTIFIER; break;
            case 51: if (c == 'a') state = 52; else return TOKEN_IDENTIFIER; break;
            case 52: if (c == 'r') state = 53; else return TOKEN_IDENTIFIER; break; // char[cite: 5]
            case 53: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'f' (flag, fixed)
            // ==========================================
            case 60:
                if (c == 'l') state = 61;
                else if (c == 'i') state = 65;
                else return TOKEN_IDENTIFIER;
                break;

            case 61: if (c == 'a') state = 62; else return TOKEN_IDENTIFIER; break;
            case 62: if (c == 'g') state = 63; else return TOKEN_IDENTIFIER; break; // flag[cite: 5]
            case 63: return TOKEN_IDENTIFIER;

            case 65: if (c == 'x') state = 66; else return TOKEN_IDENTIFIER; break;
            case 66: if (c == 'e') state = 67; else return TOKEN_IDENTIFIER; break;
            case 67: if (c == 'd') state = 68; else return TOKEN_IDENTIFIER; break; // fixed[cite: 5]
            case 68: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 't' (text)
            // ==========================================
            case 70: if (c == 'e') state = 71; else return TOKEN_IDENTIFIER; break;
            case 71: if (c == 'x') state = 72; else return TOKEN_IDENTIFIER; break;
            case 72: if (c == 't') state = 73; else return TOKEN_IDENTIFIER; break; // text[cite: 5]
            case 73: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'i' (if)
            // ==========================================
            case 80: if (c == 'f') state = 81; else return TOKEN_IDENTIFIER; break; // if[cite: 5]
            case 81: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'e' (else, emit)
            // ==========================================
            case 90:
                if (c == 'l') state = 91;
                else if (c == 'm') state = 95;
                else return TOKEN_IDENTIFIER;
                break;

            case 91: if (c == 's') state = 92; else return TOKEN_IDENTIFIER; break;
            case 92: if (c == 'e') state = 93; else return TOKEN_IDENTIFIER; break; // else[cite: 5]
            case 93: return TOKEN_IDENTIFIER;

            case 95: if (c == 'i') state = 96; else return TOKEN_IDENTIFIER; break;
            case 96: if (c == 't') state = 97; else return TOKEN_IDENTIFIER; break; // emit[cite: 6]
            case 97: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'g' (grab)
            // ==========================================
            case 110: if (c == 'r') state = 111; else return TOKEN_IDENTIFIER; break;
            case 111: if (c == 'a') state = 112; else return TOKEN_IDENTIFIER; break;
            case 112: if (c == 'b') state = 113; else return TOKEN_IDENTIFIER; break; // grab[cite: 6]
            case 113: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'p' (prompt)
            // ==========================================
            case 120: if (c == 'r') state = 121; else return TOKEN_IDENTIFIER; break;
            case 121: if (c == 'o') state = 122; else return TOKEN_IDENTIFIER; break;
            case 122: if (c == 'm') state = 123; else return TOKEN_IDENTIFIER; break;
            case 123: if (c == 'p') state = 124; else return TOKEN_IDENTIFIER; break;
            case 124: if (c == 't') state = 125; else return TOKEN_IDENTIFIER; break; // prompt[cite: 6]
            case 125: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'r' (routine, return)
            // ==========================================
            case 130:
                if (c == 'o') state = 131;
                else if (c == 'e') state = 141;
                else return TOKEN_IDENTIFIER;
                break;

            case 131: if (c == 'u') state = 132; else return TOKEN_IDENTIFIER; break;
            case 132: if (c == 't') state = 133; else return TOKEN_IDENTIFIER; break;
            case 133: if (c == 'i') state = 134; else return TOKEN_IDENTIFIER; break;
            case 134: if (c == 'n') state = 135; else return TOKEN_IDENTIFIER; break;
            case 135: if (c == 'e') state = 136; else return TOKEN_IDENTIFIER; break; // routine[cite: 6]
            case 136: return TOKEN_IDENTIFIER;

            case 141: if (c == 't') state = 142; else return TOKEN_IDENTIFIER; break;
            case 142: if (c == 'u') state = 143; else return TOKEN_IDENTIFIER; break;
            case 143: if (c == 'r') state = 144; else return TOKEN_IDENTIFIER; break;
            case 144: if (c == 'n') state = 145; else return TOKEN_IDENTIFIER; break; // return[cite: 6]
            case 145: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'A' (AND)
            // ==========================================
            case 150: if (c == 'N') state = 151; else return TOKEN_IDENTIFIER; break;
            case 151: if (c == 'D') state = 152; else return TOKEN_IDENTIFIER; break; // AND[cite: 6]
            case 152: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'O' (OR)
            // ==========================================
            case 160: if (c == 'R') state = 161; else return TOKEN_IDENTIFIER; break; // OR[cite: 6]
            case 161: return TOKEN_IDENTIFIER;

            // ==========================================
            // DFA Branch: 'N' (NOT)
            // ==========================================
            case 170: if (c == 'O') state = 171; else return TOKEN_IDENTIFIER; break;
            case 171: if (c == 'T') state = 172; else return TOKEN_IDENTIFIER; break; // NOT[cite: 6]
            case 172: return TOKEN_IDENTIFIER;

            default:
                return TOKEN_IDENTIFIER;
        }
        i++;
    }

    // Evaluate Accepting States once the string terminates ('\0')
    switch (state) {
        case 13: return TOKEN_SPIN;
        case 17: return TOKEN_SKIP;
        case 102: return TOKEN_STASH;
        case 23: return TOKEN_HOLD;
        case 27: return TOKEN_HALT;
        case 32: return TOKEN_DEC;
        case 36: return TOKEN_DIV_OP;
        case 42: return TOKEN_NUM;
        case 53: return TOKEN_CHAR;
        case 63: return TOKEN_FLAG;
        case 68: return TOKEN_FIXED;
        case 73: return TOKEN_TEXT;
        case 81: return TOKEN_IF;
        case 93: return TOKEN_ELSE;
        case 97: return TOKEN_EMIT;
        case 113: return TOKEN_GRAB;
        case 125: return TOKEN_PROMPT;
        case 136: return TOKEN_ROUTINE;
        case 145: return TOKEN_RETURN;
        case 152: return TOKEN_AND;
        case 161: return TOKEN_OR;
        case 172: return TOKEN_NOT;
        default: return TOKEN_IDENTIFIER;
    }
}

// Packages the lexeme and coordinates into a Token struct[cite: 11]
Token createToken(TokenType type, const char* lexeme, int line, int column) {
    Token newToken;
    
    if (type == TOKEN_IDENTIFIER) {
        newToken.type = lookupKeyword(lexeme);
    } else {
        newToken.type = type;
    }
    
    // Allocate memory for the lexeme string dynamically without string.h
    int len = 0;
    while(lexeme[len] != '\0') len++;
    newToken.lexeme = (char*)malloc((len + 1) * sizeof(char));
    for (int j = 0; j <= len; j++) {
        newToken.lexeme[j] = lexeme[j];
    }
    
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

// Helper to print tokens to the terminal for debugging
void printToken(const Token* token) {
    printf("Token [Type: %2d, Line: %3d, Col: %3d] -> %s\n", 
           token->type, token->line, token->column, token->lexeme);
}