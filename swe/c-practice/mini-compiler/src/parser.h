#ifndef PARSER_H
#define PARSER_H

#include "ast.h"
#include "lexer.h"

#define MAX_ERRORS 64

typedef struct {
    int line;
    char message[256];
} ParseError;

typedef struct {
    Lexer lx;
    Token cur;
    ParseError errors[MAX_ERRORS];
    int error_count;
} Parser;

void parser_init(Parser *p, const char *src);
/* Parses the whole file as one implicit top-level block (the program's body). */
Stmt *parser_parse_program(Parser *p);

#endif
