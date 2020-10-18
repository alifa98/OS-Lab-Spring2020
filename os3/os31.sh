#!/bin/bash

if [ ! $# -eq 2 ]
then
    echo "Error in input parameters"
    exit
fi

echo $(($1 + $2))

if [ $1 -gt $2 ]; then
    echo "larger is $1"
else
    echo "larger is $2"
fi
