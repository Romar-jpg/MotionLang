#include <stdio.h>
#include <stdlib.h>
#include "motionlang_tokens.h"
#include "stream.h"

extern SourceStream *lexer_stream;
extern Token getNextToken(void);

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
        return 1; // Valid
    }
    
    return 0; // Invalid
}

int main(int argc, char *argv[]) {
    // Determine input file (use command line arg or default to test.mtnl)
    const char* inputFile = (argc > 1) ? argv[1] : "test.mtnl";

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

    fprintf(outputFile, "MOTIONLANG SYMBOL TABLE\n");
    fprintf(outputFile, "======================================================\n");
    fprintf(outputFile, "%-15s | %-20s | %s\n", "TOKEN CLASSIFICATION", "LEXEME", "COORDINATES");
    fprintf(outputFile, "======================================================\n");

    Token t;
    do {
        t = getNextToken();
        
        // Write the mapped entries directly to the text file
        fprintf(outputFile, "Type: %-10d | Lexeme: %-15s | Line: %3d, Col: %3d\n", 
                t.type, t.lexeme, t.line, t.column);
        
        freeToken(&t);
    } while (t.type != TOKEN_EOF);

    fclose(outputFile);
    close_stream(lexer_stream);

    printf("Lexical analysis complete! Output generated in 'Symbol Table.txt'.\n");
    return 0;
}