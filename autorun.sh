#!/usr/bin/env bash
WATCH="$1"
CMD="$2"

$CMD
inotifywait -e close_write,moved_to,create -mq $WATCH |
while read -r directory events filename; do
    $CMD
done
