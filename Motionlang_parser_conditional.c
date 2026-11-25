/*
 * Motionlang_parser_conditional.c
 * MotionLang - Syntactic Phase: CONDITIONALS
 *
 * EBNF:
 *   if_stmt = "If" expr [ "Then" ] block
 *             { "Else" "If" expr [ "Then" ] block }
 *             [ "Else" block ] ;
 */

#include <stdio.h>
#include "Motionlang_parser.h"

void parse_if(void) {
    advance();                                   /* eat "If"                  */
    char *cond = parse_expr();                   /* the condition             */
    out("IF");
    depth++;
    out("CONDITION %s", cond);
    accept_noise(N_THEN);                        /* optional "Then"           */
    parse_block("THEN-BLOCK");                   /* { ... }                   */
    depth--;

    for (;;) {                                   /* zero or more Else / Else If */
        int save = pos;                          /* remember where we are     */
        skip_newlines();                         /* "Else" may be on next line */
        if (!check(K_ELSE)) {                    /* no Else -> put things back */
            pos = save;
            return;
        }
        advance();                               /* eat "Else"                */

        depth++;
        if (match(K_IF)) {                       /* "Else If" */
            char *c = parse_expr();
            out("ELSE-IF");
            depth++;
            out("CONDITION %s", c);
            accept_noise(N_THEN);                /* optional "Then"           */
            parse_block("THEN-BLOCK");
            depth--;
            depth--;
        } else {                                 /* plain "Else" - last branch */
            out("ELSE");
            parse_block("ELSE-BLOCK");
            depth--;
            return;
        }
    }
}
