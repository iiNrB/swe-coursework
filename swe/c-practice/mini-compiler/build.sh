#!/bin/sh
# Builds the mc compiler itself from src/*.c using whatever gcc is on PATH.
set -e
cd "$(dirname "$0")"
gcc -std=c11 -Wall -O2 src/lexer.c src/parser.c src/codegen.c src/main.c -o mc
echo "Built ./mc"
