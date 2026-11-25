#include <string.h>
#include <ctype.h>
#include "motionlang_parser.h"

/*
 * NOISE WORDS  (Then / Do)
 *
 * Optional filler words. The lexer gives them the single token type
 * TOKEN_NOISE; the lexeme tells us which word it is.
 *   "then" -> allowed after an If / Else If condition
 *   "do"   -> allowed after a Spin (loop) header
 *
 * <noise> -> "then" | "do"        (always optional, never in the tree)
 */

// Case-insensitive string compare (strcasecmp is not portable to Windows).
// NULL-safe: a NULL string never matches.
static int same_word(const char *a, const char *b) {
    if (a == NULL || b == NULL) return 0;
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b)) return 0;
        a++; b++;
    }
    return *a == '\0' && *b == '\0';
}

/*
 * Optional noise word: if the current token is the noise word `word`,
 * skip it. Otherwise do nothing (so a different noise word is left in
 * place for the caller to report).
 */
void accept_noise(Parser *parser, const char *word) {
    if (check(parser, TOKEN_NOISE) && same_word(peek(parser).lexeme, word)) {
        advance(parser);
    }
}

/*
 * Reports a noise word that is NOT allowed where it appears (e.g. "then"
 * at the start of a statement, or "do" after an If condition).
 * The stray word is skipped so the parser always makes progress.
 */
void noise_misplaced(Parser *parser) {
    syntax_error(parser,
        "Noise word is only allowed after an if/else-if condition (then) "
        "or a loop header (do).");
    advance(parser);
}
