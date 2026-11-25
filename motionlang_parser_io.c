#include <motionlang_parser.h>


// ===================================================================================================
//                                  INPUT STATEMENT
//                <input_stmt> -> "grab" <IDENTIFIER> "=" "prompt" "(" <STRING> ")" ";"
// ===================================================================================================

void parse_input_stmt(Parser *parser) {
    // Expect and consume the 'grab' keyword (TOKEN_GRAB)
    if (!consume(parser, TOKEN_GRAB, "Expected 'grab' at the start of input statement.")) {
        recover(parser);
        return;
    }

    // Expect and consume the target variable name (<IDENTIFIER>)
    if (!consume(parser, TOKEN_IDENTIFIER, "Expected <IDENTIFIER> after 'grab'.")) {
        recover(parser);
        return;
    }

    // Expect and consume the '=' assignment operator (TOKEN_ASSIGN)
    if (!consume(parser, TOKEN_ASSIGN, "Expected '=' after identifier in input statement.")) {
        recover(parser);
        return;
    }

    // Expect and consume the 'prompt' keyword (TOKEN_PROMPT)
    if (!consume(parser, TOKEN_PROMPT, "Expected 'prompt' keyword after '='.")) {
        recover(parser);
        return;
    }

    // Expect and consume the opening parenthesis '(' (TOKEN_LPAREN)
    if (!consume(parser, TOKEN_LPAREN, "Expected '(' after 'prompt'.")) {
        recover(parser);
        return;
    }

    // Expect and consume the opening double quote '"' (TOKEN_DOUBLE_QUOTE)
    if (!consume(parser, TOKEN_DOUBLE_QUOTE, "Expected opening '\"' for prompt string.")) {
        recover(parser);
        return;
    }

    // Expect and consume the prompt message (<STRING>)
    if (!consume(parser, TOKEN_STRING_LITERAL, "Expected string literal inside 'prompt(...)'.")) {
        recover(parser);
        return;
    }

    // Expect and consume the closing double quote '"' (TOKEN_DOUBLE_QUOTE)
    if (!consume(parser, TOKEN_DOUBLE_QUOTE, "Expected closing '\"' after prompt string.")) {
        recover(parser);
        return;
    }

    // Expect and consume the closing parenthesis ')' (TOKEN_RPAREN)
    if (!consume(parser, TOKEN_RPAREN, "Expected ')' after prompt string.")) {
        recover(parser);
        return;
    }

    // Expect and consume the closing semicolon ';' (TOKEN_SEMICOLON)
    if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' at the end of input statement.")) {
        recover(parser);
        return;
    }
}


// ===================================================================================================
//                                  OUTPUT STATEMENT
//                             <output_stmt> -> "emit" <expr> ";"
// ===================================================================================================

void parse_output_stmt(Parser *parser) {

    // Expect and consume the 'emit' keyword (TOKEN_EMIT)
    if (!consume(parser, TOKEN_EMIT, "Expected 'emit' at the start of output statement.")) {
        recover(parser);
        return;
    }

    // Parse the expression to be emitted (<expr>)
    parse_expr(parser);

    // If an error happened inside the expression, recover before checking ';'
    if (parser->panic_mode) {
        recover(parser);
        return;
    }

    // Expect and consume the closing semicolon ';' (TOKEN_SEMICOLON)
    if (!consume(parser, TOKEN_SEMICOLON, "Expected ';' at the end of output statement.")) {
        recover(parser);
        return;
    }
}


