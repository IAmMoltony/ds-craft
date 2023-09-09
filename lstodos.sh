#!/bin/sh
clear
grep -rn ./source ./include -e 'TODO' -e '@todo' --color
