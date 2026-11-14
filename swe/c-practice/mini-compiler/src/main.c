#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "codegen.h"

static char *read_file(const char *path) {
    FILE *f = fopen(path, "rb");
    if (!f) { fprintf(stderr, "mc: cannot open '%s'\n", path); exit(1); }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    char *buf = malloc(size + 1);
    fread(buf, 1, size, f);
    buf[size] = '\0';
    fclose(f);
    return buf;
}

static int cmp_err_line(const void *a, const void *b) {
    return ((ParseError *)a)->line - ((ParseError *)b)->line;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: mc <source.c> [-o <output.s>]\n");
        return 1;
    }
    const char *in_path = argv[1];
    const char *out_path = "out.s";
    for (int i = 2; i < argc - 1; i++)
        if (argv[i][0] == '-' && argv[i][1] == 'o') out_path = argv[i + 1];

    char *src = read_file(in_path);

    Parser p;
    parser_init(&p, src);
    Stmt *program = parser_parse_program(&p);

    if (p.error_count > 0) {
        qsort(p.errors, p.error_count, sizeof(ParseError), cmp_err_line);
        fprintf(stderr, "%d syntax error(s) in %s:\n", p.error_count, in_path);
        for (int i = 0; i < p.error_count; i++)
            fprintf(stderr, "  line %d: %s\n", p.errors[i].line, p.errors[i].message);
        return 1;
    }

    FILE *out = fopen(out_path, "w");
    if (!out) { fprintf(stderr, "mc: cannot write '%s'\n", out_path); return 1; }

    Codegen cg;
    codegen_init(&cg, out);
    int ok = codegen_generate(&cg, program);
    fclose(out);

    if (!ok) {
        qsort(cg.errors, cg.error_count, sizeof(CodegenError), cmp_err_line);
        fprintf(stderr, "%d error(s) in %s:\n", cg.error_count, in_path);
        for (int i = 0; i < cg.error_count; i++)
            fprintf(stderr, "  line %d: %s\n", cg.errors[i].line, cg.errors[i].message);
        remove(out_path);
        return 1;
    }

    fprintf(stderr, "OK: wrote %s\n", out_path);
    return 0;
}
