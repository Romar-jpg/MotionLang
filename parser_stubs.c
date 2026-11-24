#include <stdio.h>
#include "motionlang_parser.h"

// Temporary helper so your dispatcher can step through statements during solo testing
static void skip_until_statement_end(Parser *parser) {
    while (!check(parser, TOKEN_EOF) &&
           !check(parser, TOKEN_SEMICOLON) &&
           !check(parser, TOKEN_LBRACE) &&
           !check(parser, TOKEN_RBRACE)) {
        advance(parser);
    }
    if (check(parser, TOKEN_SEMICOLON)) {
        advance(parser);
    } else if (check(parser, TOKEN_LBRACE)) {
        parse_block(parser);
    }
}

void parse_declaration_stmt(Parser *parser) { skip_until_statement_end(parser); }
void parse_assign_stmt(Parser *parser)      { skip_until_statement_end(parser); }
void parse_input_stmt(Parser *parser)       { skip_until_statement_end(parser); }
void parse_output_stmt(Parser *parser)      { skip_until_statement_end(parser); }
void parse_if_stmt(Parser *parser) {
    skip_until_statement_end(parser);
    while (check(parser, TOKEN_ELSE_IF) || check(parser, TOKEN_ELSE)) {
        skip_until_statement_end(parser);
    }
}
void parse_loop_stmt(Parser *parser)        { skip_until_statement_end(parser); }
void parse_flow_stmt(Parser *parser)        { skip_until_statement_end(parser); }
void parse_routine_decl(Parser *parser)     { skip_until_statement_end(parser); }
