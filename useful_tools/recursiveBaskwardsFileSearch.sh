#!/bin/bash

function testAndExecute(){
    if test -x "$1"; then
        FILE_NAME=$1
        shift
        echo "executing $FILE_NAME $* in $(pwd)"
        "$FILE_NAME" "$@"
        exit 0
    elif test -f "$1"; then
        echo "file foud in $(pwd) but it is not executable"
    fi
}

if test $# -lt 1; then
    echo "usage: $0 <file to execute> {arguments for file}"
    exit 1
fi
EXECUTABLE=$1
shift


testAndExecute "$EXECUTABLE" "$@"
while [ "$(pwd)" != "/" ]; do
    cd ..
    testAndExecute "$EXECUTABLE" "$@"
done

echo "file not foud: $EXECUTABLE"

exit 1


