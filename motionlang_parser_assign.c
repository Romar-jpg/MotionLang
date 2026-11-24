#include "motionlang_parser.h"

/*
 * Parses an assignment statement:
 * <assignment_stmt> -> <IDENTIFIER> = <expr> ;
 */
void parse_assign_stmt(Parser *parser) {
    // 1. Expect and consume the variable name (<IDENTIFIER>)
    if (!consume(parser, TOKEN_IDENTIFIER, "Expected an identifier at the start of assignment statement.")) {
        recover(parser);
        return;
    }

    // 2. Expect and consume the '=' operator (TOKEN_ASSIGN)
    if (!consume(parser, TOKEN_ASSIGN, "Expected '=' after identifier in assignment statement.")) {
        recover(parser);
        return;
    }

    // 3. Parse the right-hand side expression (<expr>)
    parse_expr(parser);

    // If an error happened inside the expression, recover before checking ';'
    if (parser->panic_mode) {
        recover(parser);
        return;
    }

    // 4. Expect and consume the closing semicolon (';')
    if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' at the end of assignment statement.")) {
        recover(parser);
        return;
    }
}
