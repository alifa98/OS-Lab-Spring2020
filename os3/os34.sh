#!/bin/bash
#sh script.sh f a b
#print from line a till b in file f

cat $1 | head -n $3 | tail -n $(($3-$2+1))