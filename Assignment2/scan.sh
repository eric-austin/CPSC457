#!/bin/bash

find . -name "*.$1" -printf "%p %s\n" | sort -k 2 -n -r | head -n $2 | awk '{ x += $2 ; print $1 " " $2 } END { print "Total size: " x }'
