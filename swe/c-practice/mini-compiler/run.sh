#!/bin/sh
# One-command build-and-run: ./run.sh examples/hello.c
# - builds ./mc if it doesn't exist yet
# - compiles the given .c file with mc into out.s
#   (if mc finds syntax/semantic errors, they're printed with line numbers and nothing runs)
# - assembles+links out.s with gcc and runs the resulting program
set -e
cd "$(dirname "$0")"

if [ -z "$1" ]; then
    echo "usage: ./run.sh <file.c>"
    exit 1
fi

if [ ! -x ./mc ] && [ ! -f ./mc.exe ]; then
    ./build.sh
fi
MC=./mc
[ -f ./mc.exe ] && MC=./mc.exe

if ! "$MC" "$1" -o out.s; then
    exit 1
fi

gcc out.s -o out_program
./out_program
