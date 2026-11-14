#ifndef AST_H
#define AST_H

typedef enum { TY_INT, TY_FLOAT } VarType;

typedef enum {
    E_NUM_INT, E_NUM_FLOAT, E_VAR,
    E_BINOP, E_UNOP
} ExprKind;

/* Binary/unary operators share one op code space. */
typedef enum {
    OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_MOD,
    OP_LT, OP_GT, OP_LE, OP_GE, OP_EQ, OP_NE,
    OP_AND, OP_OR,
    OP_NEG, OP_POS, OP_NOT
} OpKind;

typedef struct Expr {
    ExprKind kind;
    int line;
    VarType type;        /* filled in by the type-checking pass in codegen */
    long ival;            /* E_NUM_INT */
    double fval;           /* E_NUM_FLOAT */
    char name[64];        /* E_VAR */
    OpKind op;             /* E_BINOP / E_UNOP */
    struct Expr *lhs, *rhs; /* rhs unused for E_UNOP */
} Expr;

typedef enum {
    S_DECL, S_ASSIGN, S_IF, S_WHILE, S_PRINTF, S_BLOCK
} StmtKind;

typedef struct Stmt {
    StmtKind kind;
    int line;

    /* S_DECL */
    VarType decl_type;
    char decl_name[64];
    Expr *init;           /* optional initializer, may be NULL */

    /* S_ASSIGN */
    char assign_name[64];
    Expr *assign_value;

    /* S_IF / S_WHILE */
    Expr *cond;
    struct Stmt *then_branch;
    struct Stmt *else_branch; /* S_IF only, may be NULL */

    /* S_PRINTF */
    char fmt[256];
    Expr *print_arg;      /* NULL if format string has no specifier */

    /* S_BLOCK */
    struct Stmt **body;
    int body_count;

    struct Stmt *next;     /* sibling link inside a block's body is NOT used; body[] is used instead */
} Stmt;

#endif
