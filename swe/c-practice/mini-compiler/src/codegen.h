#ifndef CODEGEN_H
#define CODEGEN_H

#include <stdio.h>
#include "ast.h"

#define MAX_CODEGEN_ERRORS 64

typedef struct {
    int line;
    char message[256];
} CodegenError;

typedef struct {
    FILE *out;
    CodegenError errors[MAX_CODEGEN_ERRORS];
    int error_count;
} Codegen;

void codegen_init(Codegen *cg, FILE *out);
/* Returns 1 on success (assembly written), 0 if semantic errors were found
   (in which case nothing usable was written and cg->errors holds them). */
int codegen_generate(Codegen *cg, Stmt *program);

#endif
