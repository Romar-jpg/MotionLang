#include <stdio.h>
#include <stdlib.h>
#include "motionlang_tokens.h"

TokenType lookupKeyword(const char* lexeme) {
    int state = 0;
    int i = 0;

    while (lexeme[i] != '\0') {
        char c = lexeme[i];

        switch (state) {
            case 0:
                if (c == 's') state = 10;
                else if (c == 'h') state = 20;
                else if (c == 'd') state = 30;
                else if (c == 'n') state = 40;
                else if (c == 'c') state = 50;
                else if (c == 'f') state = 60;
                else if (c == 't') state = 70;
                else if (c == 'i') state = 80;
                else if (c == 'e') state = 90;
                else if (c == 'g') state = 110;
                else if (c == 'p') state = 120;
                else if (c == 'r') state = 130;
                else if (c == 'A') state = 150;
                else if (c == 'O') state = 160;
                else if (c == 'N') state = 170;
                else return TOKEN_IDENTIFIER;
                break;

            case 10:
                if (c == 'p') state = 11;
                else if (c == 'k') state = 15;
                else if (c == 't') state = 18;
                else return TOKEN_IDENTIFIER;
                break;

            case 11: if (c == 'i') state = 12; else return TOKEN_IDENTIFIER; break;
            case 12: if (c == 'n') state = 13; else return TOKEN_IDENTIFIER; break;
            case 13: return TOKEN_IDENTIFIER;

            case 15: if (c == 'i') state = 16; else return TOKEN_IDENTIFIER; break;
            case 16: if (c == 'p') state = 17; else return TOKEN_IDENTIFIER; break;
            case 17: return TOKEN_IDENTIFIER;

            case 18: if (c == 'a') state = 19; else return TOKEN_IDENTIFIER; break;
            case 19: if (c == 's') state = 101; else return TOKEN_IDENTIFIER; break;
            case 101: if (c == 'h') state = 102; else return TOKEN_IDENTIFIER; break;
            case 102: return TOKEN_IDENTIFIER;

            case 20:
                if (c == 'o') state = 21;
                else if (c == 'a') state = 25;
                else return TOKEN_IDENTIFIER;
                break;

            case 21: if (c == 'l') state = 22; else return TOKEN_IDENTIFIER; break;
            case 22: if (c == 'd') state = 23; else return TOKEN_IDENTIFIER; break;
            case 23: return TOKEN_IDENTIFIER;

            case 25: if (c == 'l') state = 26; else return TOKEN_IDENTIFIER; break;
            case 26: if (c == 't') state = 27; else return TOKEN_IDENTIFIER; break;
            case 27: return TOKEN_IDENTIFIER;

            case 30:
                if (c == 'e') state = 31;
                else if (c == 'i') state = 35;
                else return TOKEN_IDENTIFIER;
                break;

            case 31: if (c == 'c') state = 32; else return TOKEN_IDENTIFIER; break;
            case 32: return TOKEN_IDENTIFIER;

            case 35: if (c == 'v') state = 36; else return TOKEN_IDENTIFIER; break;
            case 36: return TOKEN_IDENTIFIER;

            case 40: if (c == 'u') state = 41; else return TOKEN_IDENTIFIER; break;
            case 41: if (c == 'm') state = 42; else return TOKEN_IDENTIFIER; break;
            case 42: return TOKEN_IDENTIFIER;

            case 50: if (c == 'h') state = 51; else return TOKEN_IDENTIFIER; break;
            case 51: if (c == 'a') state = 52; else return TOKEN_IDENTIFIER; break;
            case 52: if (c == 'r') state = 53; else return TOKEN_IDENTIFIER; break;
            case 53: return TOKEN_IDENTIFIER;

            case 60:
                if (c == 'l') state = 61;
                else if (c == 'i') state = 65;
                else return TOKEN_IDENTIFIER;
                break;

            case 61: if (c == 'a') state = 62; else return TOKEN_IDENTIFIER; break;
            case 62: if (c == 'g') state = 63; else return TOKEN_IDENTIFIER; break;
            case 63: return TOKEN_IDENTIFIER;

            case 65: if (c == 'x') state = 66; else return TOKEN_IDENTIFIER; break;
            case 66: if (c == 'e') state = 67; else return TOKEN_IDENTIFIER; break;
            case 67: if (c == 'd') state = 68; else return TOKEN_IDENTIFIER; break;
            case 68: return TOKEN_IDENTIFIER;

            case 70: if (c == 'e') state = 71; else return TOKEN_IDENTIFIER; break;
            case 71: if (c == 'x') state = 72; else return TOKEN_IDENTIFIER; break;
            case 72: if (c == 't') state = 73; else return TOKEN_IDENTIFIER; break;
            case 73: return TOKEN_IDENTIFIER;

            case 80: if (c == 'f') state = 81; else return TOKEN_IDENTIFIER; break;
            case 81: return TOKEN_IDENTIFIER;

            case 90:
                if (c == 'l') state = 91;
                else if (c == 'm') state = 95;
                else return TOKEN_IDENTIFIER;
                break;

            case 91: if (c == 's') state = 92; else return TOKEN_IDENTIFIER; break;
            case 92: if (c == 'e') state = 93; else return TOKEN_IDENTIFIER; break;
            case 93: return TOKEN_IDENTIFIER;

            case 95: if (c == 'i') state = 96; else return TOKEN_IDENTIFIER; break;
            case 96: if (c == 't') state = 97; else return TOKEN_IDENTIFIER; break;
            case 97: return TOKEN_IDENTIFIER;

            case 110: if (c == 'r') state = 111; else return TOKEN_IDENTIFIER; break;
            case 111: if (c == 'a') state = 112; else return TOKEN_IDENTIFIER; break;
            case 112: if (c == 'b') state = 113; else return TOKEN_IDENTIFIER; break;
            case 113: return TOKEN_IDENTIFIER;

            case 120: if (c == 'r') state = 121; else return TOKEN_IDENTIFIER; break;
            case 121: if (c == 'o') state = 122; else return TOKEN_IDENTIFIER; break;
            case 122: if (c == 'm') state = 123; else return TOKEN_IDENTIFIER; break;
            case 123: if (c == 'p') state = 124; else return TOKEN_IDENTIFIER; break;
            case 124: if (c == 't') state = 125; else return TOKEN_IDENTIFIER; break;
            case 125: return TOKEN_IDENTIFIER;

            case 130:
                if (c == 'o') state = 131;
                else if (c == 'e') state = 141;
                else return TOKEN_IDENTIFIER;
                break;

            case 131: if (c == 'u') state = 132; else return TOKEN_IDENTIFIER; break;
            case 132: if (c == 't') state = 133; else return TOKEN_IDENTIFIER; break;
            case 133: if (c == 'i') state = 134; else return TOKEN_IDENTIFIER; break;
            case 134: if (c == 'n') state = 135; else return TOKEN_IDENTIFIER; break;
            case 135: if (c == 'e') state = 136; else return TOKEN_IDENTIFIER; break;
            case 136: return TOKEN_IDENTIFIER;

            case 141: if (c == 't') state = 142; else return TOKEN_IDENTIFIER; break;
            case 142: if (c == 'u') state = 143; else return TOKEN_IDENTIFIER; break;
            case 143: if (c == 'r') state = 144; else return TOKEN_IDENTIFIER; break;
            case 144: if (c == 'n') state = 145; else return TOKEN_IDENTIFIER; break;
            case 145: return TOKEN_IDENTIFIER;

            case 150: if (c == 'N') state = 151; else return TOKEN_IDENTIFIER; break;
            case 151: if (c == 'D') state = 152; else return TOKEN_IDENTIFIER; break;
            case 152: return TOKEN_IDENTIFIER;

            case 160: if (c == 'R') state = 161; else return TOKEN_IDENTIFIER; break;
            case 161: return TOKEN_IDENTIFIER;

            case 170: if (c == 'O') state = 171; else return TOKEN_IDENTIFIER; break;
            case 171: if (c == 'T') state = 172; else return TOKEN_IDENTIFIER; break;
            case 172: return TOKEN_IDENTIFIER;

            default:
                return TOKEN_IDENTIFIER;
        }
        i++;
    }

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

Token createToken(TokenType type, const char* lexeme, int line, int column) {
    Token newToken;

    if (type == TOKEN_IDENTIFIER) {
        newToken.type = lookupKeyword(lexeme);
    } else {
        newToken.type = type;
    }

    int len = 0;
    while (lexeme[len] != '\0') len++;
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

void printToken(const Token* token) {
    printf("Token [Type: %2d, Line: %3d, Col: %3d] -> %s\n",
           token->type, token->line, token->column, token->lexeme);
}