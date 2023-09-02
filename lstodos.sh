#!/bin/sh
clear
grep -rnw ./source ./include -e 'TODO' -e '@todo' --color
