#!/usr/bin/env bash

function echocmd () {
	local cmd="$*"
	echo -e "\u001b[38;5;245m$ $cmd \u001b[0m"
	eval $cmd
	return $?
}

echocmd \
    gcc --std=c99 -Wall -Werror -Wfloat-equal -Wfloat-conversion -Wpedantic \
        ./src/* \
    && (
        echocmd ./a.out
    )
