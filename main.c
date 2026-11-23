#include <stdio.h>
#include <stdlib.h>
#include "motionlang_tokens.h"
#include "stream.h"

extern SourceStream *lexer_stream;
extern Token getNextToken(void);

// Helper function to map integer enum types to descriptive text
const char* getTokenClassificationName(TokenType type) {
    switch (type) {
        // Data Types
        case TOKEN_NUM:             return "DATA_TYPE_NUM";
        case TOKEN_DEC:             return "DATA_TYPE_DEC";
        case TOKEN_CHAR:            return "DATA_TYPE_CHAR";
        case TOKEN_FLAG:            return "DATA_TYPE_FLAG";
        case TOKEN_TEXT:            return "DATA_TYPE_TEXT";
        case TOKEN_STASH:           return "DATA_TYPE_STASH";

        // Storage Constraints
        case TOKEN_HOLD:            return "STORAGE_HOLD";
        case TOKEN_FIXED:           return "STORAGE_FIXED";

        // Condition and teration
        case TOKEN_IF:              return "KEYWORD_IF";
        case TOKEN_ELSE:            return "KEYWORD_ELSE";
        case TOKEN_ELSE_IF:         return "KEYWORD_ELSE_IF";
        case TOKEN_SPIN:            return "KEYWORD_SPIN";
        case TOKEN_HALT:            return "KEYWORD_HALT";
        case TOKEN_SKIP:            return "KEYWORD_SKIP";

        // Input/Output & Functions
        case TOKEN_GRAB:            return "KEYWORD_GRAB";
        case TOKEN_EMIT:            return "KEYWORD_EMIT";
        case TOKEN_PROMPT:          return "KEYWORD_PROMPT";
        case TOKEN_ROUTINE:         return "KEYWORD_ROUTINE";
        case TOKEN_RETURN:          return "KEYWORD_RETURN";
        // Word Operators
        case TOKEN_DIV_OP:          return "OPERATOR_DIV";
        case TOKEN_AND:             return "LOGICAL_AND";
        case TOKEN_OR:              return "LOGICAL_OR";
        case TOKEN_NOT:             return "LOGICAL_NOT";

        // Identifiers & Literals
        case TOKEN_IDENTIFIER:      return "IDENTIFIER";
        case TOKEN_INTEGER_LITERAL: return "INTEGER_LITERAL";
        case TOKEN_DECIMAL_LITERAL: return "DECIMAL_LITERAL";
        case TOKEN_STRING_LITERAL:  return "STRING_LITERAL";
        case TOKEN_CHAR_LITERAL:    return "CHAR_LITERAL";
        case TOKEN_NOISE:           return "NOISE_WORD";

        // Arithmetic & Relational Symbols
        case TOKEN_PLUS:            return "OPERATOR_PLUS";
        case TOKEN_MINUS:           return "OPERATOR_MINUS";
        case TOKEN_STAR:            return "OPERATOR_MULTIPLY";
        case TOKEN_SLASH:           return "OPERATOR_DIVIDE";
        case TOKEN_MODULO:          return "OPERATOR_MODULO";
        case TOKEN_CARET:           return "OPERATOR_EXPONENT";
        case TOKEN_ASSIGN:          return "ASSIGNMENT_OPERATOR";
        case TOKEN_EQUAL:           return "RELATIONAL_EQUAL";
        case TOKEN_NOT_EQUAL:       return "RELATIONAL_NOTEQUAL";
        case TOKEN_LESS:            return "RELATIONAL_LESS";
        case TOKEN_GREATER:         return "RELATIONAL_GREATER";
        case TOKEN_LESS_EQUAL:      return "RELATIONAL_LESSEQUAL";
        case TOKEN_GREATER_EQUAL:   return "RELATIONAL_GREATEREQUAL";

        // Delimiters & Brackets
        case TOKEN_LPAREN:          return "DELIM_LPAREN";
        case TOKEN_RPAREN:          return "DELIM_RPAREN";
        case TOKEN_LBRACKET:        return "DELIM_LBRACKET";
        case TOKEN_RBRACKET:        return "DELIM_RBRACKET";
        case TOKEN_LBRACE:          return "DELIM_LBRACE";
        case TOKEN_RBRACE:          return "DELIM_RBRACE";
        case TOKEN_COMMA:           return "DELIM_COMMA";
        case TOKEN_SEMICOLON:       return "DELIM_SEMICOLON";
        case TOKEN_SINGLE_QUOTE:    return "DELIM_SINGLE_QUOTE";
        case TOKEN_DOUBLE_QUOTE:    return "DELIM_DOUBLE_QUOTE";

        // Comments
        case TOKEN_COMMENT:         return "COMMENT";

        // End of File / Unknown
        case TOKEN_UNKNOWN:         return "LEXICAL_ERROR";
        case TOKEN_EOF:             return "END_OF_FILE";
        default:                    return "UNKNOWN_TOKEN";
    }
}

// Manual validation to ensure file ends strictly in ".mtnl"
int isValidFileExtension(const char* filename) {
    int len = 0;
    while (filename[len] != '\0') {
        len++;
    }
    
    if (len < 5) return 0; // File name too short to contain ".mtnl"
    
    if (filename[len-5] == '.' && 
        filename[len-4] == 'm' && 
        filename[len-3] == 't' && 
        filename[len-2] == 'n' && 
        filename[len-1] == 'l') {
        return 1;
    }
    
    return 0;
}

// Prints a token row to Symbol Table.txt, wrapping long lexemes cleanly across lines
void writeFormattedTokenRow(FILE *out, const Token *t) {
    const int COL_WIDTH = 55;
    const char *className = getTokenClassificationName(t->type);
    const char *lex = t->lexeme;

    // Count total characters in lexeme
    int len = 0;
    while (lex[len] != '\0') {
        len++;
    }

    // If it fits within the column width, print a single normal row
    if (len <= COL_WIDTH) {
        fprintf(out, "%-26s | %-55s | Line: %-3d Col: %-3d\n",
                className, lex, t->line, t->column);
        return;
    }

    // Otherwise, print the first chunk with classification and coordinates
    int offset = 0;
    while (offset < len) {
        char chunk[56];
        int remaining = len - offset;
        int take = (remaining > COL_WIDTH) ? COL_WIDTH : remaining;

        // Try to break cleanly at a space if we aren't at the end of the string
        if (remaining > COL_WIDTH) {
            int last_space = -1;
            for (int i = 0; i < take; i++) {
                if (lex[offset + i] == ' ') {
                    last_space = i;
                }
            }
            if (last_space > 0) {
                take = last_space + 1; // Include the space at the end of the wrapped line
            }
        }

        for (int i = 0; i < take; i++) {
            chunk[i] = lex[offset + i];
        }
        chunk[take] = '\0';

        if (offset == 0) {
            fprintf(out, "%-26s | %-55s | Line: %-3d Col: %-3d\n",
                    className, chunk, t->line, t->column);
        } else {
            // Continuation rows keep the outer columns blank so the table grid stays straight
            fprintf(out, "%-26s | %-55s |\n", "", chunk);
        }

        offset += take;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_file.mtnl>\n", argv[0]);
        return 1;
    }

    const char* inputFile = argv[1];

    printf("Validating file type for: %s\n", inputFile);
    if (!isValidFileExtension(inputFile)) {
        printf("ERROR: Incompatible or unauthorized file type.\n");
        printf("MotionLang Lexer only accepts '.mtnl' files.\n");
        return 1;
    }

    lexer_stream = init_stream(inputFile);
    if (!lexer_stream) {
        printf("ERROR: Could not open %s\n", inputFile);
        return 1;
    }

    // Open the mandatory output file
    FILE *outputFile = fopen("Symbol Table.txt", "w");
    if (!outputFile) {
        printf("ERROR: Could not create 'Symbol Table.txt'\n");
        close_stream(lexer_stream);
        return 1;
    }

   // Aligned header layout
    fprintf(outputFile, "====================================================================================================================\n");
    fprintf(outputFile, "%-26s | %-55s | %s\n", "TOKEN CLASSIFICATION", "LEXEME", "COORDINATES");
    fprintf(outputFile, "====================================================================================================================\n");

   Token t;
    do {
        t = getNextToken();
        writeFormattedTokenRow(outputFile, &t);
        freeToken(&t);
    } while (t.type != TOKEN_EOF);
    
    fprintf(outputFile, "====================================================================================================================\n");

    fclose(outputFile);
    close_stream(lexer_stream);

    printf("Lexical analysis complete! Output generated in 'Symbol Table.txt'.\n");
    return 0;
}