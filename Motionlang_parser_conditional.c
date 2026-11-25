#include "motionlang_parser.h"

/*
 * CONDITIONALS
 *
 * <if_stmt>   -> "If" <cond_body>
 *                { ( "Else If" | "Else" "If" ) <cond_body> }
 *                [ "Else" <block> ]
 * <cond_body> -> <expr> [ "then" ] <block>
 *
 * (TOKEN_ELSE_IF is the lexer's single "Else If" token. A separate
 *  TOKEN_ELSE followed by TOKEN_IF is also accepted.)
 *
 * Error convention (same as parse_assign_stmt):
 *   - parse_expr() / consume() only FLAG the error (panic_mode = 1).
 *   - Whoever sees panic_mode still set calls recover() ONCE and returns.
 *   - If a nested call (parse_block) already recovered, panic_mode is
 *     back to 0, so we carry on and do not skip extra tokens.
 *   - A broken header (bad condition / wrong noise word) skips its whole
 *     block, so the Else branches after it are still parsed normally.
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

// <cond_body> -> <expr> [ "then" ] <block>
// Returns 1 if parsing of the if-statement can continue (Else branches etc.),
// 0 if we must stop (block failed and panic mode is still set).
static int parse_cond_body(Parser *parser) {
    parse_expr(parser);                       // the condition
    if (parser->panic_mode) {
        skip_control_block(parser, 1);
        return 1;
    }

    accept_noise(parser, "then");             // optional noise word

    if (check(parser, TOKEN_NOISE)) {         // wrong noise word, e.g. "do"
        noise_misplaced(parser);
        skip_control_block(parser, 0);
        return 1;
    }

    parse_block(parser);                      // { ... }
    if (parser->panic_mode) return 0;         // block could not recover itself

    return 1;
}

void parse_if_stmt(Parser *parser) {
    // 1. "If"
    if (!consume(parser, TOKEN_IF, "Expected 'If'.")) {
        recover(parser);
        return;
    }

    // 2. condition [then] { ... }
    if (!parse_cond_body(parser)) {
        if (parser->panic_mode) recover(parser);
        return;
    }

    // 3. zero or more "Else If" branches
    while (check(parser, TOKEN_ELSE_IF) ||
           (check(parser, TOKEN_ELSE) && peek_next(parser).type == TOKEN_IF)) {

        if (check(parser, TOKEN_ELSE_IF)) {
            advance(parser);                  // single "Else If" token
        } else {
            advance(parser);                  // "Else"
            advance(parser);                  // "If"
        }

        if (!parse_cond_body(parser)) {
            if (parser->panic_mode) recover(parser);
            return;
        }
    }

    // 4. optional final "Else" { ... }
    if (match(parser, TOKEN_ELSE)) {
        parse_block(parser);
        if (parser->panic_mode) {
            recover(parser);
            return;
        }
    }
}
