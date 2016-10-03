#!/bin/sh

filename="knapsack"
testfile="test5.txt"

touch $filename.txt
if [ -e $filename.txt ]
then
    cat /dev/null >| $filename.txt
fi

for i in 1 2 4 8 16;
do
    echo "Running with $i thread(s)..."
    (time --format "Elapsed time: %e" ./$filename $testfile $i) 2>&1 | tee -a $filename.txt
    echo "\tdone."
done

