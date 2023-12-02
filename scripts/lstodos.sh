#!/bin/sh
clear
grep -rn ./source ./include ./scripts/*.py ./README.md -e 'TODO' -e '@todo' --color
