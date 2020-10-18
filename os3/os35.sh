#!/bin/bash

if [ ! $# -eq 1 ]; then
    echo "please pass an integer in list 1, 2 or 3"
    exit
fi

if [ $1 -eq 1 ]; then
    for i in 1 2 3 4 5; do
        a=1
        while [ $a -le $i ]; do
            printf "%s" $i
            a=$((a + 1))
        done
        echo
    done
fi

if [ $1 -eq 2 ]; then
    for ((i = 1; i <= 6; i++)); do
        for ((j = 1; j <= 6 - i; j++)); do
            echo -n "  "
        done
        for ((j = 1; j <= 2 * i - 1; j++)); do
            echo -n "* "
        done
        echo
    done

    for ((i = 6; i >= 0; i--)); do
        for ((j = 1; j <= 6 - i; j++)); do
            echo -n "  "
        done
        for ((j = 1; j <= 2 * i - 1; j++)); do
            echo -n "* "
        done
        echo
    done
fi

if [ $1 -eq 3 ]; then
    for ((i = 1; i <= 5; i++)); do
        for ((j = 1; j <= i; j++)); do
            if [ $j -eq $i ]; then
                echo -n "|_"
            else
                echo -n "|"
            fi
        done
        echo
    done
fi
