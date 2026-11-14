#ifndef LEXER_H
#define LEXER_H

typedef enum {
    T_EOF, T_INT, T_FLOAT, T_IF, T_ELSE, T_WHILE, T_PRINTF,
    T_IDENT, T_NUMBER, T_STRING,
    T_PLUS, T_MINUS, T_STAR, T_SLASH, T_PERCENT,
    T_ASSIGN, T_SEMI, T_COMMA,
    T_LPAREN, T_RPAREN, T_LBRACE, T_RBRACE,
    T_LT, T_GT, T_LE, T_GE, T_EQ, T_NE,
    T_AND, T_OR, T_NOT,
    T_UNKNOWN
} TokenType;

typedef struct {
    TokenType type;
    char text[256];   /* raw lexeme: identifier name, number text, or string contents */
    int line;
} Token;

typedef struct {
    const char *src;
    int pos;
    int line;
} Lexer;

void lexer_init(Lexer *lx, const char *src);
/* Fills *out with the next token and advances. */
void lexer_next(Lexer *lx, Token *out);

#endif
