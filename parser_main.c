#include <stdio.h>
#include "motionlang_parser.h"

int main(int argc, char *argv[]) {
    const char *symbol_file = (argc > 1) ? argv[1] : "Symbol Table.txt";

    Parser parser;
    if (!load_tokens_from_symbol_table(symbol_file, &parser)) {
        return 1;
    }

    printf("Loaded %d tokens from '%s'. Starting syntax analysis...\n", parser.count, symbol_file);
    parse_program(&parser);

    if (parser.error_count == 0) {
        printf("Syntax analysis successful! 0 syntax errors found.\n");
    } else {
        printf("Syntax analysis completed with %d syntax error(s).\n", parser.error_count);
    }

    free_parser(&parser);
    return (parser.error_count == 0) ? 0 : 1;
}