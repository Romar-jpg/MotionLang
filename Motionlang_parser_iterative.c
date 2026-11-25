/*
 * Motionlang_parser_iterative.c
 * MotionLang - Syntactic Phase: ITERATIVE STATEMENTS (loops)
 *
 * EBNF:
 *   while_stmt = "While" expr [ "Do" ] block ;
 *   for_stmt   = "For" simple_stmt ";" expr ";" assign [ "Do" ] block ;
 *
 * NOTE: "While" and "For" are not in the proposal's keyword list yet.
 *       Add them to the keyword registry (Pt. 3).
 */

#include <stdio.h>
#include "Motionlang_parser.h"

/* ---- While ---- */
void parse_while(void) {
    advance();                                   /* eat "While"          */
    char *cond = parse_expr();
    out("WHILE");
    depth++;
    out("CONDITION %s", cond);
    accept_noise(N_DO);                          /* optional "Do"        */
    loop_depth++;                                /* Break/Continue legal */
    parse_block("BODY");
    loop_depth--;
    depth--;
}

/* ---- For init ; cond ; step ---- */
void parse_for(void) {
    advance();                                   /* eat "For"            */
    out("FOR");
    depth++;

    out("INIT:");
    depth++; parse_simple(); depth--;            /* e.g. Num i = 0       */
    expect(T_SEMI, "';' after for-initializer");

    char *cond = parse_expr();                   /* e.g. i < 5           */
    out("CONDITION %s", cond);
    expect(T_SEMI, "';' after for-condition");

    out("STEP:");
    depth++;
    if (!(check(T_IDENT) && toks[pos + 1].type == T_ASSIGN))
        fail("for-step must be an assignment (e.g. i = i + 1)");
    parse_simple();                              /* e.g. i = i + 1       */
    depth--;

    accept_noise(N_DO);                          /* optional "Do"        */
    loop_depth++;
    parse_block("BODY");
    loop_depth--;
    depth--;
}
