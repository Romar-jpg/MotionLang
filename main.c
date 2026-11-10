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

int main(int argc, char *argv[]) {
    const char* inputFile = (argc > 1) ? argv[1] : "valid_test.mtnl";

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
    fprintf(outputFile, "=========================================================================================\n");
    fprintf(outputFile, "%-26s | %-24s | %s\n", "TOKEN CLASSIFICATION", "LEXEME", "COORDINATES");
    fprintf(outputFile, "=========================================================================================\n");

    Token t;
    do {
        t = getNextToken();
        
        // Writes descriptive token classification rather than raw enum integers
        fprintf(outputFile, "%-26s | %-24s | Line: %-3d Col: %-3d\n", 
                getTokenClassificationName(t.type), 
                t.lexeme, 
                t.line, 
                t.column);
        
        freeToken(&t);
    } while (t.type != TOKEN_EOF);

    fprintf(outputFile, "=========================================================================================\n");

    fclose(outputFile);
    close_stream(lexer_stream);

    printf("Lexical analysis complete! Output generated in 'Symbol Table.txt'.\n");
    return 0;
}