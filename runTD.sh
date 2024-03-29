#!/bin/bash

#
# The following script uses the Leasot npm package to
# parse files in immediate dir and subdirs for todo
# and fixme comments. Output can be modified under the
# variable 'OUT'. Please refer to Leasot documentation
# to see list of supporting files.
#
# Nobel Truong 2019
#

CC=g++
DST=$(git config --get remote.origin.url)
ROOT=$(git rev-parse --show-toplevel)

NON="No todos/fixmes found"
NOF="No files found for parsing"
OUT="todo_list"

: > $OUT.txt

shopt -s globstar
for f in $ROOT/**/*
do
    if leasot $f | grep -q "$NON"  
    then
        continue
    elif leasot $f | grep -q "NOF"
    then
        continue
    elif leasot $f | grep "Filetype .* is unsupported."
    then
        continue
    else
        leasot $f >> $OUT.txt
    fi
done

$CC -o toweb toweb.cpp

./toweb -i $(pwd)/$OUT.txt -o $ROOT/$OUT.html -d $DST -r $ROOT

