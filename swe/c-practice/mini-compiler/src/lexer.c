#include <ctype.h>
#include <string.h>
#include "lexer.h"

void lexer_init(Lexer *lx, const char *src) {
    lx->src = src;
    lx->pos = 0;
    lx->line = 1;
}

static int peekc(Lexer *lx) { return lx->src[lx->pos]; }
static int peekc2(Lexer *lx) { return lx->src[lx->pos] ? lx->src[lx->pos + 1] : 0; }

static void advance(Lexer *lx) {
    if (lx->src[lx->pos] == '\n') lx->line++;
    lx->pos++;
}

static void skip_ws_and_comments(Lexer *lx) {
    for (;;) {
        int c = peekc(lx);
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            advance(lx);
        } else if (c == '/' && peekc2(lx) == '/') {
            while (peekc(lx) && peekc(lx) != '\n') advance(lx);
        } else if (c == '/' && peekc2(lx) == '*') {
            advance(lx); advance(lx);
            while (peekc(lx) && !(peekc(lx) == '*' && peekc2(lx) == '/')) advance(lx);
            if (peekc(lx)) { advance(lx); advance(lx); }
        } else {
            break;
        }
    }
}

void lexer_next(Lexer *lx, Token *out) {
    skip_ws_and_comments(lx);
    out->line = lx->line;
    out->text[0] = '\0';

    int c = peekc(lx);
    if (c == 0) { out->type = T_EOF; return; }

    if (isalpha(c) || c == '_') {
        int start = lx->pos, n = 0;
        while (isalnum(peekc(lx)) || peekc(lx) == '_') { advance(lx); n++; }
        int len = n < 255 ? n : 255;
        memcpy(out->text, lx->src + start, len);
        out->text[len] = '\0';
        if (strcmp(out->text, "int") == 0) out->type = T_INT;
        else if (strcmp(out->text, "float") == 0) out->type = T_FLOAT;
        else if (strcmp(out->text, "if") == 0) out->type = T_IF;
        else if (strcmp(out->text, "else") == 0) out->type = T_ELSE;
        else if (strcmp(out->text, "while") == 0) out->type = T_WHILE;
        else if (strcmp(out->text, "printf") == 0) out->type = T_PRINTF;
        else out->type = T_IDENT;
        return;
    }

    if (isdigit(c) || (c == '.' && isdigit(peekc2(lx)))) {
        int start = lx->pos, n = 0;
        while (isdigit(peekc(lx))) { advance(lx); n++; }
        if (peekc(lx) == '.') {
            advance(lx); n++;
            while (isdigit(peekc(lx))) { advance(lx); n++; }
        }
        if (peekc(lx) == 'f' || peekc(lx) == 'F') { advance(lx); n++; } /* 2.5f suffix */
        int len = n < 255 ? n : 255;
        memcpy(out->text, lx->src + start, len);
        out->text[len] = '\0';
        out->type = T_NUMBER;
        return;
    }

    if (c == '"') {
        advance(lx);
        int n = 0;
        while (peekc(lx) && peekc(lx) != '"' && n < 255) {
            int ch = peekc(lx);
            if (ch == '\\') {
                advance(lx);
                int esc = peekc(lx);
                if (esc == 'n') ch = '\n';
                else if (esc == 't') ch = '\t';
                else if (esc == '\\') ch = '\\';
                else if (esc == '"') ch = '"';
                else ch = esc;
                advance(lx);
            } else {
                advance(lx);
            }
            out->text[n++] = (char)ch;
        }
        if (peekc(lx) == '"') advance(lx);
        out->text[n] = '\0';
        out->type = T_STRING;
        return;
    }

    switch (c) {
        case '+': advance(lx); out->type = T_PLUS; return;
        case '-': advance(lx); out->type = T_MINUS; return;
        case '*': advance(lx); out->type = T_STAR; return;
        case '/': advance(lx); out->type = T_SLASH; return;
        case '%': advance(lx); out->type = T_PERCENT; return;
        case ';': advance(lx); out->type = T_SEMI; return;
        case ',': advance(lx); out->type = T_COMMA; return;
        case '(': advance(lx); out->type = T_LPAREN; return;
        case ')': advance(lx); out->type = T_RPAREN; return;
        case '{': advance(lx); out->type = T_LBRACE; return;
        case '}': advance(lx); out->type = T_RBRACE; return;
        case '!':
            advance(lx);
            if (peekc(lx) == '=') { advance(lx); out->type = T_NE; } else out->type = T_NOT;
            return;
        case '=':
            advance(lx);
            if (peekc(lx) == '=') { advance(lx); out->type = T_EQ; } else out->type = T_ASSIGN;
            return;
        case '<':
            advance(lx);
            if (peekc(lx) == '=') { advance(lx); out->type = T_LE; } else out->type = T_LT;
            return;
        case '>':
            advance(lx);
            if (peekc(lx) == '=') { advance(lx); out->type = T_GE; } else out->type = T_GT;
            return;
        case '&':
            advance(lx);
            if (peekc(lx) == '&') { advance(lx); out->type = T_AND; return; }
            out->type = T_UNKNOWN; out->text[0] = '&'; out->text[1] = '\0';
            return;
        case '|':
            advance(lx);
            if (peekc(lx) == '|') { advance(lx); out->type = T_OR; return; }
            out->type = T_UNKNOWN; out->text[0] = '|'; out->text[1] = '\0';
            return;
        default:
            advance(lx);
            out->type = T_UNKNOWN;
            out->text[0] = (char)c; out->text[1] = '\0';
            return;
    }
}
