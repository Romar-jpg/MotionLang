#include "motionlang_parser.h"

/*
 * declaration_stmt
 *   = storage data_type declaration_list ";"
 *
 * declaration_list
 *   = decl_item { "," decl_item }
 */

static int parse_storage(Parser *parser) {
    if (match(parser, TOKEN_HOLD)) return 1;
    if (match(parser, TOKEN_FIXED)) return 1;

    syntax_error(parser, "Expected 'hold' or 'fixed' before declaration.");
    return 0;
}

static int parse_data_type(Parser *parser) {
    if (match(parser, TOKEN_NUM) ||
        match(parser, TOKEN_DEC) ||
        match(parser, TOKEN_CHAR) ||
        match(parser, TOKEN_FLAG) ||
        match(parser, TOKEN_TEXT)) {
        return 1;
    }

    if (match(parser, TOKEN_STASH)) {
        if (match(parser, TOKEN_LBRACKET)) {
            if (!consume(parser, TOKEN_INTEGER_LITERAL,
                         "Expected an integer array size after '['.")) {
                return 0;
            }
            if (!consume(parser, TOKEN_RBRACKET,
                         "Expected ']' after array size.")) {
                return 0;
            }
        }

        /* MotionLang also permits "stash num name[size]". */
        if (check(parser, TOKEN_NUM) || check(parser, TOKEN_DEC) ||
            check(parser, TOKEN_CHAR) || check(parser, TOKEN_FLAG) ||
            check(parser, TOKEN_TEXT)) {
            advance(parser);
        }
        return 1;
    }

    syntax_error(parser,
                 "Expected a data type: num, dec, char, flag, text, or stash.");
    return 0;
}

static void parse_decl_item(Parser *parser) {
    if (!consume(parser, TOKEN_IDENTIFIER,
                 "Expected an identifier in the declaration list.")) {
        return;
    }

    /* Array declarations may use name[size] after the identifier. */
    if (match(parser, TOKEN_LBRACKET)) {
        if (!consume(parser, TOKEN_INTEGER_LITERAL,
                     "Expected an integer array size after '['.")) {
            return;
        }
        consume(parser, TOKEN_RBRACKET, "Expected ']' after array size.");
    }

    if (match(parser, TOKEN_ASSIGN)) {
        parse_expr(parser);
    }
}

static void parse_declaration_list(Parser *parser) {
    parse_decl_item(parser);

    while (match(parser, TOKEN_COMMA)) {
        parse_decl_item(parser);
    }
}

void parse_declaration_stmt(Parser *parser) {
    if (!parse_storage(parser) || parser->panic_mode) return;
    if (!parse_data_type(parser) || parser->panic_mode) return;

    parse_declaration_list(parser);
    if (!parser->panic_mode) {
        consume(parser, TOKEN_SEMICOLON,
                "Expected ';' at the end of declaration.");
    }
}

void parse_routine_decl(Parser *parser) {
    if (!consume(parser, TOKEN_ROUTINE,
                 "Expected 'routine' at the start of declaration.")) {
        return;
    }
    if (!consume(parser, TOKEN_IDENTIFIER, "Expected routine name.")) {
        return;
    }
    if (!consume(parser, TOKEN_LPAREN,
                 "Expected '(' after routine name.")) {
        return;
    }

    if (!check(parser, TOKEN_RPAREN)) {
        do {
            if (!parse_data_type(parser) || parser->panic_mode) {
                return;
            }
            if (!consume(parser, TOKEN_IDENTIFIER,
                         "Expected parameter name.")) {
                return;
            }
        } while (match(parser, TOKEN_COMMA));
    }

    if (!consume(parser, TOKEN_RPAREN,
                 "Expected ')' after routine parameters.")) {
        return;
    }

    parse_block(parser);
}
