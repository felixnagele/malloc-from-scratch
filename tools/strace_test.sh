#!/bin/bash

TEST_BINARY=$1
STRACE_OUTPUT=$(mktemp)
trap 'rm -f "$STRACE_OUTPUT"' EXIT

strace -e trace=brk "$TEST_BINARY" 2> "$STRACE_OUTPUT" > /dev/null

TEST_EXIT=$?
if [ $TEST_EXIT -ne 0 ]; then
    exit $TEST_EXIT
fi

# Compare first and last brk(0x...) argument addresses to check for heap changes.
FIRST=$(grep "brk(0x" "$STRACE_OUTPUT" | head -1 | grep -oE "0x[0-9a-f]+" | head -1)
LAST=$(grep "brk(0x" "$STRACE_OUTPUT" | tail -1 | grep -oE "0x[0-9a-f]+" | head -1)

if [ -z "$FIRST" ] && [ -z "$LAST" ]; then
    echo "Memory OK: no brk allocations detected"
    exit 0
fi

if [ "$FIRST" = "$LAST" ]; then
    echo "Memory OK: $FIRST == $LAST"
    exit 0
else
    echo "Memory leak: $FIRST != $LAST"
    exit 1
fi
