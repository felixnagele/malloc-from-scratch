#!/bin/bash

TEST_BINARY=$1
STRACE_OUTPUT=$(mktemp)

strace -e trace=brk "$TEST_BINARY" 2> "$STRACE_OUTPUT" > /dev/null

TEST_EXIT=$?
if [ $TEST_EXIT -ne 0 ]; then
    rm -f "$STRACE_OUTPUT"
    exit $TEST_EXIT
fi

FIRST=$(grep "brk(0x" "$STRACE_OUTPUT" | head -1 | grep -o "0x[0-9a-f]*")
LAST=$(grep "brk(0x" "$STRACE_OUTPUT" | tail -1 | grep -o "0x[0-9a-f]*")

rm -f "$STRACE_OUTPUT"

if [ "$FIRST" == "$LAST" ]; then
    echo "Memory OK: $FIRST == $LAST"
    exit 0
else
    echo "Memory leak: $FIRST != $LAST"
    exit 1
fi
