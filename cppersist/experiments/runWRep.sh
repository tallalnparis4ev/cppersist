#!/bin/bash
#$1 = number of runs
#$2 = recursion num input
#$3 = iterative num input
#$4 = seq/wrep/worep

#5 input, use cppersist, recursive, keep cache, seed, worep/wrep/seq 
#5 1 1 0 0 worep/wrep/seq
#Recursive
for i in $(seq "$1")
do
  ./cppersist $2 0 1 1 $i wrep
done
#Iterative
for i in $(seq "$1")
do
  ./cppersist $3 0 0 1 $i wrep
done

./deleteAndRun $1 $2 $3 wrep
./runNoDelete $1 $2 $3 wrep 

