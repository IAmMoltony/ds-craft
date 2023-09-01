#!/bin/sh
grep -rnw ./source ./include -e 'TODO' -e '@todo' --color
