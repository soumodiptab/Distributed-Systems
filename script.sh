#!/bin/bash
file="./assignment-1/2.out"
for i in {1..12}
do
    echo "No of processes : $i"
    mpirun --use-hwthread-cpus -np $i --oversubscribe $file
done