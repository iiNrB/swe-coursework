#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "codegen.h"

/* ---- symbol table (one flat global scope -- declarations are hoisted,
   which keeps this learning subset simple: no nested lexical scoping) ---- */
typedef struct { char name[64]; VarType type; } Symbol;
#define MAX_SYMS 256
static Symbol g_syms[MAX_SYMS];
static int g_sym_count;

/* string literals used by printf(), collected so they can be emitted once
   in a read-only data section */
#define MAX_STRINGS 128
static char g_strings[MAX_STRINGS][256];
static int g_string_count;

static int g_label_counter;
static Codegen *g_cg; /* convenience for the error helper below */

static void cg_error(int line, const char *msg) {
    for (int i = 0; i < g_cg->error_count; i++)
        if (g_cg->errors[i].line == line && strcmp(g_cg->errors[i].message, msg) == 0) return;
    if (g_cg->error_count < MAX_CODEGEN_ERRORS) {
        g_cg->errors[g_cg->error_count].line = line;
        snprintf(g_cg->errors[g_cg->error_count].message, 256, "%s", msg);
        g_cg->error_count++;
    }
}

static Symbol *find_sym(const char *name) {
    for (int i = 0; i < g_sym_count; i++)
        if (strcmp(g_syms[i].name, name) == 0) return &g_syms[i];
    return NULL;
}

static void add_sym(const char *name, VarType type, int line) {
    if (find_sym(name)) {
        char msg[300];
        snprintf(msg, sizeof(msg), "variable '%s' is declared more than once", name);
        cg_error(line, msg);
        return;
    }
    if (g_sym_count < MAX_SYMS) {
        snprintf(g_syms[g_sym_count].name, 64, "%s", name);
        g_syms[g_sym_count].type = type;
        g_sym_count++;
    }
}

static int add_string(const char *s) {
    for (int i = 0; i < g_string_count; i++)
        if (strcmp(g_strings[i], s) == 0) return i; /* reuse identical literals */
    int idx = g_string_count++;
    snprintf(g_strings[idx], 256, "%s", s);
    return idx;
}

static void collect_decls(Stmt *s) {
    if (!s) return;
    switch (s->kind) {
        case S_DECL:
            add_sym(s->decl_name, s->decl_type, s->line);
            break;
        case S_BLOCK:
            for (int i = 0; i < s->body_count; i++) collect_decls(s->body[i]);
            break;
        case S_IF:
            collect_decls(s->then_branch);
            collect_decls(s->else_branch);
            break;
        case S_WHILE:
            collect_decls(s->then_branch);
            break;
        default:
            break;
    }
}

static VarType type_of(Expr *e) {
    switch (e->kind) {
        case E_NUM_INT: return TY_INT;
        case E_NUM_FLOAT: return TY_FLOAT;
        case E_VAR: {
            Symbol *sym = find_sym(e->name);
            if (!sym) {
                char msg[300];
                snprintf(msg, sizeof(msg), "variable '%s' was never declared", e->name);
                cg_error(e->line, msg);
                return TY_INT;
            }
            return sym->type;
        }
        case E_UNOP:
            if (e->op == OP_NOT) return TY_INT;
            return type_of(e->lhs);
        case E_BINOP:
            switch (e->op) {
                case OP_LT: case OP_GT: case OP_LE: case OP_GE:
                case OP_EQ: case OP_NE: case OP_AND: case OP_OR:
                    return TY_INT;
                default: {
                    VarType l = type_of(e->lhs), r = type_of(e->rhs);
                    if (e->op == OP_MOD) {
                        if (l != TY_INT || r != TY_INT)
                            cg_error(e->line, "the '%' operator requires integer operands");
                        return TY_INT;
                    }
                    return (l == TY_FLOAT || r == TY_FLOAT) ? TY_FLOAT : TY_INT;
                }
            }
    }
    return TY_INT;
}

static uint32_t float_bits(double v) {
    float f = (float)v;
    uint32_t bits;
    memcpy(&bits, &f, 4);
    return bits;
}

static void gen_expr(Codegen *cg, Expr *e);

/* Evaluates e and leaves a 0/1 truth value in %eax, regardless of e's type. */
static void gen_bool(Codegen *cg, Expr *e) {
    VarType t = type_of(e);
    gen_expr(cg, e);
    if (t == TY_INT) {
        fprintf(cg->out, "    cmp $0, %%eax\n    setne %%al\n    movzbl %%al, %%eax\n");
    } else {
        fprintf(cg->out, "    pxor %%xmm1, %%xmm1\n    ucomiss %%xmm1, %%xmm0\n    setne %%al\n    movzbl %%al, %%eax\n");
    }
}

static void gen_expr(Codegen *cg, Expr *e) {
    switch (e->kind) {
        case E_NUM_INT:
            fprintf(cg->out, "    mov $%ld, %%eax\n", e->ival);
            return;
        case E_NUM_FLOAT:
            fprintf(cg->out, "    mov $0x%08x, %%eax\n    movd %%eax, %%xmm0\n", float_bits(e->fval));
            return;
        case E_VAR: {
            Symbol *sym = find_sym(e->name);
            VarType t = sym ? sym->type : TY_INT;
            if (t == TY_FLOAT)
                fprintf(cg->out, "    movss var_%s(%%rip), %%xmm0\n", e->name);
            else
                fprintf(cg->out, "    mov var_%s(%%rip), %%eax\n", e->name);
            return;
        }
        case E_UNOP: {
            VarType t = type_of(e->lhs);
            if (e->op == OP_NOT) { gen_bool(cg, e->lhs); fprintf(cg->out, "    xor $1, %%eax\n"); return; }
            gen_expr(cg, e->lhs);
            if (e->op == OP_POS) return;
            /* OP_NEG */
            if (t == TY_FLOAT)
                fprintf(cg->out, "    mov $0x80000000, %%eax\n    movd %%eax, %%xmm1\n    pxor %%xmm1, %%xmm0\n");
            else
                fprintf(cg->out, "    neg %%eax\n");
            return;
        }
        case E_BINOP: {
            if (e->op == OP_AND || e->op == OP_OR) {
                int n = g_label_counter++;
                gen_bool(cg, e->lhs);
                fprintf(cg->out, "    cmp $0, %%eax\n");
                if (e->op == OP_AND) {
                    fprintf(cg->out, "    je L_false_%d\n", n);
                    gen_bool(cg, e->rhs);
                    fprintf(cg->out, "    jmp L_end_%d\nL_false_%d:\n    mov $0, %%eax\nL_end_%d:\n", n, n, n);
                } else {
                    fprintf(cg->out, "    jne L_true_%d\n", n);
                    gen_bool(cg, e->rhs);
                    fprintf(cg->out, "    jmp L_end_%d\nL_true_%d:\n    mov $1, %%eax\nL_end_%d:\n", n, n, n);
                }
                return;
            }

            VarType lt = type_of(e->lhs), rt = type_of(e->rhs);
            int result_float = (lt == TY_FLOAT || rt == TY_FLOAT);
            int is_relational = (e->op == OP_LT || e->op == OP_GT || e->op == OP_LE ||
                                  e->op == OP_GE || e->op == OP_EQ || e->op == OP_NE);

            gen_expr(cg, e->lhs);
            if (lt == TY_FLOAT) fprintf(cg->out, "    sub $8, %%rsp\n    movss %%xmm0, (%%rsp)\n");
            else                 fprintf(cg->out, "    push %%rax\n");

            gen_expr(cg, e->rhs);
            if (result_float && rt != TY_FLOAT)
                fprintf(cg->out, "    cvtsi2ss %%eax, %%xmm0\n");

            if (lt == TY_FLOAT) { fprintf(cg->out, "    movss (%%rsp), %%xmm1\n    add $8, %%rsp\n"); }
            else {
                fprintf(cg->out, "    pop %%rcx\n");
                if (result_float) fprintf(cg->out, "    cvtsi2ss %%ecx, %%xmm1\n");
            }
            /* now: left value is in xmm1 (float case) or ecx (int case); right is in xmm0 or eax */

            if (result_float) {
                if (is_relational) {
                    fprintf(cg->out, "    ucomiss %%xmm0, %%xmm1\n");
                    const char *cc =
                        e->op == OP_LT ? "setb" : e->op == OP_GT ? "seta" :
                        e->op == OP_LE ? "setbe" : e->op == OP_GE ? "setae" :
                        e->op == OP_EQ ? "sete" : "setne";
                    fprintf(cg->out, "    %s %%al\n    movzbl %%al, %%eax\n", cc);
                } else {
                    const char *op = e->op == OP_ADD ? "addss" : e->op == OP_SUB ? "subss" :
                                      e->op == OP_MUL ? "mulss" : "divss";
                    fprintf(cg->out, "    %s %%xmm0, %%xmm1\n    movaps %%xmm1, %%xmm0\n", op);
                }
            } else {
                if (is_relational) {
                    fprintf(cg->out, "    cmp %%eax, %%ecx\n");
                    const char *cc =
                        e->op == OP_LT ? "setl" : e->op == OP_GT ? "setg" :
                        e->op == OP_LE ? "setle" : e->op == OP_GE ? "setge" :
                        e->op == OP_EQ ? "sete" : "setne";
                    fprintf(cg->out, "    %s %%al\n    movzbl %%al, %%eax\n", cc);
                } else if (e->op == OP_ADD) {
                    fprintf(cg->out, "    add %%ecx, %%eax\n");
                } else if (e->op == OP_SUB) {
                    fprintf(cg->out, "    sub %%eax, %%ecx\n    mov %%ecx, %%eax\n");
                } else if (e->op == OP_MUL) {
                    fprintf(cg->out, "    imul %%ecx, %%eax\n");
                } else if (e->op == OP_DIV || e->op == OP_MOD) {
                    fprintf(cg->out, "    mov %%eax, %%ebx\n    mov %%ecx, %%eax\n    cdq\n    idiv %%ebx\n");
                    if (e->op == OP_MOD) fprintf(cg->out, "    mov %%edx, %%eax\n");
                }
            }
            return;
        }
    }
}

static void gen_store(Codegen *cg, const char *name, VarType target_type, Expr *value) {
    VarType vt = type_of(value);
    gen_expr(cg, value);
    if (target_type == TY_FLOAT && vt != TY_FLOAT) fprintf(cg->out, "    cvtsi2ss %%eax, %%xmm0\n");
    if (target_type == TY_INT && vt == TY_FLOAT) fprintf(cg->out, "    cvttss2si %%xmm0, %%eax\n");
    if (target_type == TY_FLOAT) fprintf(cg->out, "    movss %%xmm0, var_%s(%%rip)\n", name);
    else fprintf(cg->out, "    mov %%eax, var_%s(%%rip)\n", name);
}

static void gen_stmt(Codegen *cg, Stmt *s) {
    if (!s) return;
    switch (s->kind) {
        case S_DECL:
            if (s->init) gen_store(cg, s->decl_name, s->decl_type, s->init);
            return;
        case S_ASSIGN: {
            Symbol *sym = find_sym(s->assign_name);
            if (!sym) {
                char msg[300];
                snprintf(msg, sizeof(msg), "variable '%s' was never declared", s->assign_name);
                cg_error(s->line, msg);
                return;
            }
            gen_store(cg, s->assign_name, sym->type, s->assign_value);
            return;
        }
        case S_IF: {
            int n = g_label_counter++;
            gen_bool(cg, s->cond);
            fprintf(cg->out, "    cmp $0, %%eax\n    je L_else_%d\n", n);
            gen_stmt(cg, s->then_branch);
            fprintf(cg->out, "    jmp L_endif_%d\nL_else_%d:\n", n, n);
            if (s->else_branch) gen_stmt(cg, s->else_branch);
            fprintf(cg->out, "L_endif_%d:\n", n);
            return;
        }
        case S_WHILE: {
            int n = g_label_counter++;
            fprintf(cg->out, "L_loop_%d:\n", n);
            gen_bool(cg, s->cond);
            fprintf(cg->out, "    cmp $0, %%eax\n    je L_loopend_%d\n", n);
            gen_stmt(cg, s->then_branch);
            fprintf(cg->out, "    jmp L_loop_%d\nL_loopend_%d:\n", n, n);
            return;
        }
        case S_PRINTF: {
            int idx = add_string(s->fmt);
#ifdef _WIN32
            if (!s->print_arg) {
                fprintf(cg->out, "    lea str_%d(%%rip), %%rcx\n    sub $32, %%rsp\n    call printf\n    add $32, %%rsp\n", idx);
            } else {
                VarType at = type_of(s->print_arg);
                gen_expr(cg, s->print_arg);
                fprintf(cg->out, "    lea str_%d(%%rip), %%rcx\n", idx);
                if (at == TY_INT) {
                    fprintf(cg->out, "    movslq %%eax, %%rdx\n");
                } else {
                    fprintf(cg->out, "    cvtss2sd %%xmm0, %%xmm0\n    movq %%xmm0, %%rdx\n    movaps %%xmm0, %%xmm1\n");
                }
                fprintf(cg->out, "    sub $32, %%rsp\n    call printf\n    add $32, %%rsp\n");
            }
#else
            if (!s->print_arg) {
                fprintf(cg->out, "    lea str_%d(%%rip), %%rdi\n    xor %%eax, %%eax\n    call printf\n", idx);
            } else {
                VarType at = type_of(s->print_arg);
                gen_expr(cg, s->print_arg);
                fprintf(cg->out, "    lea str_%d(%%rip), %%rdi\n", idx);
                if (at == TY_INT) {
                    fprintf(cg->out, "    movslq %%eax, %%rsi\n    xor %%eax, %%eax\n    call printf\n");
                } else {
                    fprintf(cg->out, "    cvtss2sd %%xmm0, %%xmm0\n    mov $1, %%eax\n    call printf\n");
                }
            }
#endif
            return;
        }
        case S_BLOCK:
            for (int i = 0; i < s->body_count; i++) gen_stmt(cg, s->body[i]);
            return;
    }
}

void codegen_init(Codegen *cg, FILE *out) {
    cg->out = out;
    cg->error_count = 0;
    g_sym_count = 0;
    g_string_count = 0;
    g_label_counter = 0;
}

int codegen_generate(Codegen *cg, Stmt *program) {
    g_cg = cg;
    collect_decls(program);

    /* Type-check the whole program first (records errors, emits nothing) */
    FILE *devnull = NULL;
#ifdef _WIN32
    devnull = fopen("NUL", "w");
#else
    devnull = fopen("/dev/null", "w");
#endif
    FILE *real_out = cg->out;
    if (devnull) cg->out = devnull;
    gen_stmt(cg, program);
    cg->out = real_out;
    if (devnull) fclose(devnull);

    if (cg->error_count > 0) return 0;

    /* Reset codegen-only state that the dry run above may have advanced,
       then emit the real assembly. */
    g_label_counter = 0;
    g_string_count = 0;

    fprintf(cg->out, "    .text\n    .globl main\n");
    fprintf(cg->out, "main:\n    push %%rbp\n    mov %%rsp, %%rbp\n    sub $16, %%rsp\n");

    gen_stmt(cg, program);

    fprintf(cg->out, "    xor %%eax, %%eax\n    leave\n    ret\n\n");

    fprintf(cg->out, "    .section .rodata\n");
    for (int i = 0; i < g_string_count; i++) {
        fprintf(cg->out, "str_%d:\n    .asciz \"", i);
        for (const char *p = g_strings[i]; *p; p++) {
            if (*p == '\n') fprintf(cg->out, "\\n");
            else if (*p == '\t') fprintf(cg->out, "\\t");
            else if (*p == '"' || *p == '\\') fprintf(cg->out, "\\%c", *p);
            else fputc(*p, cg->out);
        }
        fprintf(cg->out, "\"\n");
    }

    fprintf(cg->out, "    .bss\n");
    for (int i = 0; i < g_sym_count; i++) {
        fprintf(cg->out, "    .align 4\nvar_%s:\n    .zero 4\n", g_syms[i].name);
    }

    return 1;
}
