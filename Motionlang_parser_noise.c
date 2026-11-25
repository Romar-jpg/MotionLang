/*
 * Motionlang_parser_noise.c
 * MotionLang - Syntactic Phase: NOISE WORDS
 *
 * Noise words are optional filler that make code read like English
 * but carry no meaning:
 *      "Then"  -> after an If / Else If condition
 *      "Do"    -> after a While / For header
 *
 * Rule: if the word is where it is allowed, skip it. If it shows up
 * anywhere else, it is a syntax error.
 *
 * EBNF:   if_stmt    = "If" expr [ "Then" ] block ... ;
 *         while_stmt = "While" expr [ "Do" ] block ;
 */
#include <stdio.h>
#include "Motionlang_parser.h"

/* Optional noise word: consume it if present, otherwise do nothing.
 * It never appears in the parse tree. */
void accept_noise(TokType noise) {
    if (check(noise)) {
        Token *n = advance();
        out("(noise word '%s' skipped)", n->lexeme);
    }
}

/* Called when a noise word is found where it is NOT allowed. */
void noise_misplaced(void) {
    fail("noise word '%s' is only allowed after an if/else-if condition (Then) "
         "or a loop header (Do)", peek()->lexeme);
}
