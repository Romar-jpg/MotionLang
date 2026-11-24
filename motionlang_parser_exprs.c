#include "motionlang_parser.h"
#include "motionlang_tokens.h"

//Assignment (=) is NOT handled 

// -------------------------------------------------------------------------
//<expr> -> <or_expr>
//-------------------------------------------------------------------------

void parse_expr(Parser *parser) {
    parse_or_expr(parser);
}

//-------------------------------------------------------------------------
//parse_or_expr: logical OR, Left-associative
//-------------------------------------------------------------------------
void parse_or_expr(Parser *parser) {
    parse_and_expr(parser);

    while (check(parser, TOKEN_OR)) {
        advance(parser);
        parse_and_expr(parser);
    }
}

//-------------------------------------------------------------------------
//parse_and_expr: logical AND, Left-associative
//-------------------------------------------------------------------------
void parse_and_expr(Parser *parser) {
    parse_rel_expr(parser);

    while (check(parser, TOKEN_AND)) {
        advance(parser);
        parse_rel_expr(parser);
    }
}

//-------------------------------------------------------------------------
//parse_rel_expr: relational / comparison
//Non-associative: <rel_expr> -> <add_expr> [ <relational_op> <add_expr> ]
//-------------------------------------------------------------------------
void parse_rel_expr(Parser *parser) {
    parse_add_expr(parser);

    if (check(parser, TOKEN_EQUAL)          ||
        check(parser, TOKEN_NOT_EQUAL)      ||
        check(parser, TOKEN_LESS)           ||
        check(parser, TOKEN_GREATER)        ||
        check(parser, TOKEN_LESS_EQUAL)     ||
        check(parser, TOKEN_GREATER_EQUAL)) {
        advance(parser);
        parse_add_expr(parser);
    }
}

//-------------------------------------------------------------------------
//parse_add_expr: additive, Left-associative
//-------------------------------------------------------------------------
void parse_add_expr(Parser *parser) {
    parse_mul_expr(parser);

    while (check(parser, TOKEN_PLUS) || check(parser, TOKEN_MINUS)) {
        advance(parser);
        parse_mul_expr(parser);
    }
}

//-------------------------------------------------------------------------
//parse_mul_expr: multiplicative, Left-associative.
//------------------------------------------------------------------------- 
void parse_mul_expr(Parser *parser) {
    parse_unary(parser);

    while (check(parser, TOKEN_STAR)    ||
           check(parser, TOKEN_SLASH)   ||
           check(parser, TOKEN_DIV_OP)  ||
           check(parser, TOKEN_MODULO)) {
        advance(parser);
        parse_unary(parser);
    }
}

//-------------------------------------------------------------------------
//parse_unary: unary NOT and unary minus, Right-associative
//-------------------------------------------------------------------------
void parse_unary(Parser *parser) {
    if (check(parser, TOKEN_NOT)) {
        advance(parser);
        parse_unary(parser);          /* right-associative */
        return;
    }

    if (check(parser, TOKEN_MINUS)) {
        advance(parser);
        parse_unary(parser);          /* right-associative */
        return;
    }

    parse_exponent_expr(parser);
}

//-------------------------------------------------------------------------
//parse_exponent_expr: exponentiation, ^ is right-associative
//<exponent_expr> -> <primary> [ "^" <unary> ]
//------------------------------------------------------------------------- 
void parse_exponent_expr(Parser *parser) {
    parse_primary(parser);

    if (check(parser, TOKEN_CARET)) {
        advance(parser);
        parse_unary(parser);
    }
}

//-------------------------------------------------------------------------
// parse_primary: highest level. handles literals, identifiers, and ( expr ).
 //-------------------------------------------------------------------------

void parse_primary(Parser *parser) {
    //Integer literal 
    if (check(parser, TOKEN_INTEGER_LITERAL)) {
        advance(parser);
        return;
    }

    //Decimal literal 
    if (check(parser, TOKEN_DECIMAL_LITERAL)) {
        advance(parser);
        return;
    }

    //String literal: " STRING_LITERAL " 
    if (check(parser, TOKEN_DOUBLE_QUOTE)) {
        advance(parser); 
        if (check(parser, TOKEN_STRING_LITERAL)) {
            advance(parser);
        } else {
            syntax_error(parser, "Expected string literal inside double quotes.");
            return;
        }
        consume(parser, TOKEN_DOUBLE_QUOTE, "Expected closing double quote after string literal.");
        return;
    }

    //Char literal: ' CHAR_LITERAL ' 
    if (check(parser, TOKEN_SINGLE_QUOTE)) {
        advance(parser); 
        if (check(parser, TOKEN_CHAR_LITERAL)) {
            advance(parser); 
        } else {
            syntax_error(parser, "Expected char literal inside single quotes.");
            return;
        }
        consume(parser, TOKEN_SINGLE_QUOTE, "Expected closing single quote after char literal.");
        return;
    }

    //Identifier 
    if (check(parser, TOKEN_IDENTIFIER)) {
        advance(parser);
        if (check(parser, TOKEN_LBRACKET)) {
            advance(parser); 
            parse_expr(parser);
            consume(parser, TOKEN_RBRACKET, "Expected ']' after array index expression.");
        }
        return;
    }

    // Parenthesized expression 
    if (check(parser, TOKEN_LPAREN)) {
        advance(parser);
        parse_expr(parser);
        consume(parser, TOKEN_RPAREN, "Expected ')' after expression.");
        return;
    }

    //Unexpected token
    syntax_error(parser, "Expected an expression (literal, identifier, or '(').");
}