#!/bin/bash

EXECUTABLE="knapsack"
TESTFILE="test5.txt"
THREADS="1 2 4 8 16"
T1=0
for i in $THREADS;
do
    TIME=`time ./$EXECUTABLE $i`
    if [ $1 -eq 1 ]
        then
            T1=$TIME
    fi
    echo "Running with $i Threads"
    echo "\tTime: $TIME"
    echo "\tSpeedup: `$T1/$TIME`"
    echo "Efficiency: `$T1/$TIME/$i`"
done

