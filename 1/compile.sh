#!/bin/bash
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
FILES="${DIR}/src/*.c ${DIR}/src/decimal/*.c"
OUT=$1

if [ -z "$OUT"]
then
    OUT="lab1"
fi

gcc --std=c99 -Wall -Werror -Wfloat-equal -Wfloat-conversion -Wpedantic $FILES -o $OUT \
    && echo -e "Compiled succefull\nExecutable $OUT was created"
