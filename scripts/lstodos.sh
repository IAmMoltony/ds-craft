#!/bin/sh

NO_CLEAR_FLAG=0

if [ "$1" == "--noclear" ]; then
    NO_CLEAR_FLAG=1
fi

if [ "$NO_CLEAR_FLAG" == "0" ]; then
    clear
fi

grep -rn ./source ./include ./scripts/*.py ./README.md -e 'TODO' -e '@todo' --color
