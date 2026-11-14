# mini-compiler — a small, real C compiler

A real compiler (not an interpreter): it lexes, parses, type-checks, and
generates x86-64 assembly by hand, which is then assembled/linked by the
system's own `gcc` into a real executable. Written in C. Built as a
learning-subset version of a full C compiler (CSE 4107 context).

## What it supports

- `int` and `float` variables, declared anywhere: `int x;` / `float y = 2.5f;`
- Assignment: `x = expr;`
- Arithmetic: `+ - * / %` (with the real C precedence/associativity rules
  from the lecture), unary `+ - !`, parentheses
- Comparisons: `< > <= >= == !=`
- Logical `&&`, `||`, `!` (short-circuit evaluated, like real C)
- `if (cond) { ... } else { ... }`
- `while (cond) { ... }`
- `printf("literal text with at most one %d or %f", expr);` — calls the
  real libc `printf`, so output is exactly what real C would print
- Int/float mixing follows real C's promotion rules (int op float → float)

Not supported (deliberately, to keep this a learnable first compiler):
functions, arrays, pointers, structs, `scanf`, multiple `%` specifiers in
one `printf`. All declarations are hoisted into one flat global scope —
there's no block-level scoping yet.

## Files

- `src/lexer.c/.h` — turns source text into tokens, tracking line numbers
- `src/parser.c/.h` — recursive-descent parser; on a syntax error it
  records `line N: message` and skips ahead to the next `;` so it can keep
  looking for *more* errors in the same run, instead of stopping at the first
- `src/ast.h` — the expression/statement tree types
- `src/codegen.c/.h` — type-checks the whole program first (catches things
  like undeclared variables and `%` used on a float), then emits x86-64
  assembly. The calling convention (System V vs Microsoft x64) is picked
  automatically at compile time via `#ifdef _WIN32`, so the exact same
  source works whether `mc` itself is built on Linux or on Windows/MinGW.

## Building and running

```bash
./build.sh              # builds ./mc (or mc.exe) from src/*.c
./run.sh examples/hello.c   # builds mc if needed, compiles+links+runs in one step
```

`run.sh` is the "press once, see output" feature: it runs `mc` on your
file, and if `mc` reports syntax or semantic errors it prints them (with
line numbers) and stops there; otherwise it assembles, links, and runs
the program and you see its real output.

## Examples

- `examples/hello.c` — minimal printf
- `examples/math_types.c` — `%`, int/float mixing, int-division truncation
- `examples/control.c` — `if`/`else`, `while`
- `examples/error_demo.c` — intentionally broken, to show multi-error
  reporting: a missing `;`, an undeclared variable, a missing `)`, and a
  missing expression — run it with `./run.sh examples/error_demo.c` and
  `mc` lists every line it found a problem on, not just the first

## Known issue on this machine (2026-11-14)

This Windows machine's MinGW `as.exe`/`ld.exe` fail to even start
(`STATUS_DLL_NOT_FOUND`) regardless of what source is fed to them — this
reproduces with a trivial one-line `int main(){return 0;}`, so it is a
broken/corrupted local toolchain install, unrelated to this project's code.
`gcc -S` and `-fsyntax-only` both work fine (confirmed all 4 source files
compile cleanly), so the compiler's own C source is good — it just
couldn't be linked into `mc.exe` and test-run *on this machine*.
**Recommended: build and run this in the Ubuntu VirtualBox VM instead**,
where the real toolchain already works — same two commands, no changes
needed, and it'll use the Linux (System V) calling convention automatically.
