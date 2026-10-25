#include <stdio.h>
#include "motionlang_tokens.h"

int main() {
    printf("Testing MotionLang Module 3: Token Structures & Registry\n");
    printf("--------------------------------------------------------\n");

    // Simulating Module 2 passing recognized strings to your packaging function
    Token t1 = createToken(TOKEN_IDENTIFIER, "spin", 1, 5);       // Should be identified as TOKEN_SPIN
    Token t2 = createToken(TOKEN_IDENTIFIER, "userScore", 2, 10); // Should remain TOKEN_IDENTIFIER
    Token t3 = createToken(TOKEN_IDENTIFIER, "grab", 3, 4);       // Should be identified as TOKEN_GRAB
    Token t4 = createToken(TOKEN_ASSIGN, "=", 3, 9);              // Manual operator passing

    // Print the tokens to verify the lookup registry worked
    printToken(&t1);
    printToken(&t2);
    printToken(&t3);
    printToken(&t4);

    // Free allocated memory
    freeToken(&t1);
    freeToken(&t2);
    freeToken(&t3);
    freeToken(&t4);

    return 0;
}