#include "motionlang_parser.h"

/*
 * ITERATIVE STATEMENTS  (Spin)
 *
 * Two forms, told apart by one token of lookahead:
 *
 * while-style:  <loop_stmt> -> "Spin" <expr> [ "do" ] <block>
 *
 * for-style:    <loop_stmt> -> "Spin" <assignment_stmt> <expr> ";"
 *                              <IDENTIFIER> "=" <expr> [ "do" ] <block>
 *     e.g.  Spin i = 0; i < 5; i = i + 1 do { ... }
 *     (the init assignment already ends with its own ';')
 *
 * If "Spin" is followed by  IDENTIFIER "="  it is the for-style, because an
 * expression can never begin that way (assignment is not an expression).
 *
 * Error convention (same as parse_assign_stmt):
 *   - parse_expr() / consume() only FLAG the error (panic_mode = 1).
 *   - Whoever sees panic_mode still set calls recover() ONCE and returns.
 *   - parse_assign_stmt and parse_block recover by themselves, so after
 *     them we must NOT call recover() again (it would skip good tokens).
 *   - A broken header skips its whole block (no fake errors from the body).
 */

// Error resync for a broken HEADER (condition / init / step / wrong noise word).
// Skips ahead to the block that belongs to the statement and skips the whole
// { ... } (braces are counted, so nested blocks are handled), then clears
// panic mode. This avoids a pile of fake errors from parsing the body as if
// it were new top-level statements.
//   stop_at_semicolon = 1: if a ';' comes before any '{', the block was
//   probably forgotten, so consume the ';' and stop there.
//   Use 0 for a for-style header, where ';' is a normal part of the header.
static void skip_control_block(Parser *parser, int stop_at_semicolon) {
    while (!check(parser, TOKEN_EOF) &&
           !check(parser, TOKEN_LBRACE) &&
           !check(parser, TOKEN_RBRACE)) {
        if (stop_at_semicolon && check(parser, TOKEN_SEMICOLON)) {
            advance(parser);
            parser->panic_mode = 0;
            return;
        }
        advance(parser);
    }

    if (check(parser, TOKEN_LBRACE)) {
        int nesting = 0;
        do {
            if (check(parser, TOKEN_LBRACE))      nesting++;
            else if (check(parser, TOKEN_RBRACE)) nesting--;
            advance(parser);
        } while (nesting > 0 && !check(parser, TOKEN_EOF));
    }
    parser->panic_mode = 0;
}

void parse_loop_stmt(Parser *parser) {
    // 1. "Spin"
    if (!consume(parser, TOKEN_SPIN, "Expected 'Spin'.")) {
        recover(parser);
        return;
    }

    if (check(parser, TOKEN_IDENTIFIER) &&
        peek_next(parser).type == TOKEN_ASSIGN) {

        // ---- for-style ----

        // init: parse_assign_stmt recovers by itself on error
        int errors_before = parser->error_count;
        parse_assign_stmt(parser);
        if (parser->panic_mode || parser->error_count > errors_before) {
            skip_control_block(parser, 0);
            return;
        }

        // condition
        parse_expr(parser);
        if (parser->panic_mode) { skip_control_block(parser, 0); return; }

        if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' after loop condition.")) {
            skip_control_block(parser, 0);
            return;
        }

        // step:  IDENTIFIER = <expr>   (no ';')
        if (!consume(parser, TOKEN_IDENTIFIER, "Expected an assignment as the loop step.") ||
            !consume(parser, TOKEN_ASSIGN,     "Expected '=' in the loop step.")) {
            skip_control_block(parser, 0);
            return;
        }
        parse_expr(parser);
        if (parser->panic_mode) { skip_control_block(parser, 0); return; }

    } else {
        // ---- while-style ----
        parse_expr(parser);                   // condition
        if (parser->panic_mode) { skip_control_block(parser, 1); return; }
    }

    accept_noise(parser, "do");               // optional noise word

    if (check(parser, TOKEN_NOISE)) {         // wrong noise word, e.g. "then"
        noise_misplaced(parser);
        skip_control_block(parser, 0);
        return;
    }

    parse_block(parser);                      // body { ... }
    if (parser->panic_mode) recover(parser);  // only if block didn't already
}

void parse_flow_stmt(Parser *parser) {
    if (match(parser, TOKEN_HALT) || match(parser, TOKEN_SKIP)) {
        consume(parser, TOKEN_SEMICOLON,
                "Expected ';' after 'halt' or 'skip'.");
        return;
    }

    if (!consume(parser, TOKEN_RETURN,
                 "Expected 'halt', 'skip', or 'return'.")) {
        return;
    }

    if (!check(parser, TOKEN_SEMICOLON)) {
        parse_expr(parser);
    }

    if (!parser->panic_mode) {
        consume(parser, TOKEN_SEMICOLON,
                "Expected ';' after return statement.");
    }
}
