#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"

static Expr *parse_expr(Parser *p);
static Stmt *parse_statement(Parser *p);

static void advance(Parser *p) { lexer_next(&p->lx, &p->cur); }

static const char *token_desc(Token *t) {
    static char buf[300];
    switch (t->type) {
        case T_EOF: return "end of file";
        case T_SEMI: return "';'";
        case T_RPAREN: return "')'";
        case T_LPAREN: return "'('";
        case T_RBRACE: return "'}'";
        case T_LBRACE: return "'{'";
        default:
            snprintf(buf, sizeof(buf), "'%s'", t->text[0] ? t->text : "?");
            return buf;
    }
}

static void error_at(Parser *p, int line, const char *msg) {
    if (p->error_count < MAX_ERRORS) {
        p->errors[p->error_count].line = line;
        snprintf(p->errors[p->error_count].message, 256, "%s", msg);
        p->error_count++;
    }
}

/* Skip tokens until we pass a ';' or reach a '}'/EOF, so parsing can keep
   looking for further errors instead of stopping at the first one. */
static void synchronize(Parser *p) {
    while (p->cur.type != T_EOF && p->cur.type != T_RBRACE) {
        if (p->cur.type == T_SEMI) { advance(p); return; }
        advance(p);
    }
}

static int expect(Parser *p, TokenType type, const char *what) {
    if (p->cur.type == type) { advance(p); return 1; }
    char msg[300];
    snprintf(msg, sizeof(msg), "expected %s but found %s", what, token_desc(&p->cur));
    error_at(p, p->cur.line, msg);
    return 0;
}

static Expr *new_expr(ExprKind kind, int line) {
    Expr *e = calloc(1, sizeof(Expr));
    e->kind = kind;
    e->line = line;
    return e;
}

static Expr *parse_primary(Parser *p) {
    int line = p->cur.line;
    if (p->cur.type == T_NUMBER) {
        Expr *e;
        if (strpbrk(p->cur.text, ".fF")) {
            e = new_expr(E_NUM_FLOAT, line);
            e->fval = atof(p->cur.text);
        } else {
            e = new_expr(E_NUM_INT, line);
            e->ival = atol(p->cur.text);
        }
        advance(p);
        return e;
    }
    if (p->cur.type == T_IDENT) {
        Expr *e = new_expr(E_VAR, line);
        snprintf(e->name, sizeof(e->name), "%s", p->cur.text);
        advance(p);
        return e;
    }
    if (p->cur.type == T_LPAREN) {
        advance(p);
        Expr *e = parse_expr(p);
        expect(p, T_RPAREN, "')'");
        return e;
    }
    char msg[300];
    snprintf(msg, sizeof(msg), "expected an expression but found %s", token_desc(&p->cur));
    error_at(p, line, msg);
    advance(p);
    return new_expr(E_NUM_INT, line); /* placeholder so the tree stays well-formed */
}

static Expr *parse_unary(Parser *p) {
    int line = p->cur.line;
    if (p->cur.type == T_MINUS) { advance(p); Expr *e = new_expr(E_UNOP, line); e->op = OP_NEG; e->lhs = parse_unary(p); return e; }
    if (p->cur.type == T_PLUS)  { advance(p); Expr *e = new_expr(E_UNOP, line); e->op = OP_POS; e->lhs = parse_unary(p); return e; }
    if (p->cur.type == T_NOT)   { advance(p); Expr *e = new_expr(E_UNOP, line); e->op = OP_NOT; e->lhs = parse_unary(p); return e; }
    return parse_primary(p);
}

static Expr *parse_binop_level(Parser *p, Expr *(*next)(Parser *), const TokenType *types, const OpKind *ops, int n) {
    Expr *left = next(p);
    for (;;) {
        int matched = -1;
        for (int i = 0; i < n; i++) if (p->cur.type == types[i]) { matched = i; break; }
        if (matched < 0) break;
        int line = p->cur.line;
        advance(p);
        Expr *right = next(p);
        Expr *e = new_expr(E_BINOP, line);
        e->op = ops[matched];
        e->lhs = left; e->rhs = right;
        left = e;
    }
    return left;
}

static Expr *parse_term(Parser *p) {
    TokenType types[] = { T_STAR, T_SLASH, T_PERCENT };
    OpKind ops[] = { OP_MUL, OP_DIV, OP_MOD };
    return parse_binop_level(p, parse_unary, types, ops, 3);
}
static Expr *parse_additive(Parser *p) {
    TokenType types[] = { T_PLUS, T_MINUS };
    OpKind ops[] = { OP_ADD, OP_SUB };
    return parse_binop_level(p, parse_term, types, ops, 2);
}
static Expr *parse_relational(Parser *p) {
    TokenType types[] = { T_LT, T_GT, T_LE, T_GE };
    OpKind ops[] = { OP_LT, OP_GT, OP_LE, OP_GE };
    return parse_binop_level(p, parse_additive, types, ops, 4);
}
static Expr *parse_equality(Parser *p) {
    TokenType types[] = { T_EQ, T_NE };
    OpKind ops[] = { OP_EQ, OP_NE };
    return parse_binop_level(p, parse_relational, types, ops, 2);
}
static Expr *parse_and(Parser *p) {
    TokenType types[] = { T_AND };
    OpKind ops[] = { OP_AND };
    return parse_binop_level(p, parse_equality, types, ops, 1);
}
static Expr *parse_or(Parser *p) {
    TokenType types[] = { T_OR };
    OpKind ops[] = { OP_OR };
    return parse_binop_level(p, parse_and, types, ops, 1);
}
static Expr *parse_expr(Parser *p) { return parse_or(p); }

static Stmt *new_stmt(StmtKind kind, int line) {
    Stmt *s = calloc(1, sizeof(Stmt));
    s->kind = kind;
    s->line = line;
    return s;
}

static Stmt *parse_block(Parser *p) {
    int line = p->cur.line;
    expect(p, T_LBRACE, "'{'");
    Stmt *blk = new_stmt(S_BLOCK, line);
    Stmt **items = NULL;
    int count = 0, cap = 0;
    while (p->cur.type != T_RBRACE && p->cur.type != T_EOF) {
        Stmt *s = parse_statement(p);
        if (s) {
            if (count == cap) { cap = cap ? cap * 2 : 8; items = realloc(items, cap * sizeof(Stmt *)); }
            items[count++] = s;
        }
    }
    expect(p, T_RBRACE, "'}'");
    blk->body = items;
    blk->body_count = count;
    return blk;
}

static Stmt *parse_decl(Parser *p) {
    int line = p->cur.line;
    VarType ty = (p->cur.type == T_INT) ? TY_INT : TY_FLOAT;
    advance(p);
    if (p->cur.type != T_IDENT) {
        error_at(p, p->cur.line, "expected a variable name after type");
        synchronize(p);
        return NULL;
    }
    Stmt *s = new_stmt(S_DECL, line);
    s->decl_type = ty;
    snprintf(s->decl_name, sizeof(s->decl_name), "%s", p->cur.text);
    advance(p);
    if (p->cur.type == T_ASSIGN) {
        advance(p);
        s->init = parse_expr(p);
    }
    if (!expect(p, T_SEMI, "';'")) { synchronize(p); }
    return s;
}

static Stmt *parse_assign(Parser *p) {
    int line = p->cur.line;
    Stmt *s = new_stmt(S_ASSIGN, line);
    snprintf(s->assign_name, sizeof(s->assign_name), "%s", p->cur.text);
    advance(p);
    if (!expect(p, T_ASSIGN, "'='")) { synchronize(p); return NULL; }
    s->assign_value = parse_expr(p);
    if (!expect(p, T_SEMI, "';'")) { synchronize(p); }
    return s;
}

static Stmt *parse_if(Parser *p) {
    int line = p->cur.line;
    advance(p); /* if */
    expect(p, T_LPAREN, "'('");
    Stmt *s = new_stmt(S_IF, line);
    s->cond = parse_expr(p);
    expect(p, T_RPAREN, "')'");
    s->then_branch = parse_statement(p);
    if (p->cur.type == T_ELSE) {
        advance(p);
        s->else_branch = parse_statement(p);
    }
    return s;
}

static Stmt *parse_while(Parser *p) {
    int line = p->cur.line;
    advance(p); /* while */
    expect(p, T_LPAREN, "'('");
    Stmt *s = new_stmt(S_WHILE, line);
    s->cond = parse_expr(p);
    expect(p, T_RPAREN, "')'");
    s->then_branch = parse_statement(p);
    return s;
}

static Stmt *parse_printf(Parser *p) {
    int line = p->cur.line;
    advance(p); /* printf */
    expect(p, T_LPAREN, "'('");
    Stmt *s = new_stmt(S_PRINTF, line);
    if (p->cur.type == T_STRING) {
        snprintf(s->fmt, sizeof(s->fmt), "%s", p->cur.text);
        advance(p);
    } else {
        error_at(p, p->cur.line, "expected a format string in printf(...)");
    }
    if (p->cur.type == T_COMMA) {
        advance(p);
        s->print_arg = parse_expr(p);
    }
    expect(p, T_RPAREN, "')'");
    if (!expect(p, T_SEMI, "';'")) { synchronize(p); }
    return s;
}

static Stmt *parse_statement(Parser *p) {
    switch (p->cur.type) {
        case T_INT: case T_FLOAT: return parse_decl(p);
        case T_IF: return parse_if(p);
        case T_WHILE: return parse_while(p);
        case T_PRINTF: return parse_printf(p);
        case T_LBRACE: return parse_block(p);
        case T_IDENT: return parse_assign(p);
        default: {
            char msg[300];
            snprintf(msg, sizeof(msg), "unexpected %s at the start of a statement", token_desc(&p->cur));
            error_at(p, p->cur.line, msg);
            synchronize(p);
            return NULL;
        }
    }
}

void parser_init(Parser *p, const char *src) {
    lexer_init(&p->lx, src);
    p->error_count = 0;
    advance(p);
}

Stmt *parser_parse_program(Parser *p) {
    int line = p->cur.line;
    Stmt *prog = new_stmt(S_BLOCK, line);
    Stmt **items = NULL;
    int count = 0, cap = 0;
    while (p->cur.type != T_EOF) {
        Stmt *s = parse_statement(p);
        if (s) {
            if (count == cap) { cap = cap ? cap * 2 : 16; items = realloc(items, cap * sizeof(Stmt *)); }
            items[count++] = s;
        }
    }
    prog->body = items;
    prog->body_count = count;
    return prog;
}
