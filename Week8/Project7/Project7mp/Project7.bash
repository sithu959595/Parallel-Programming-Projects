#!/bin/bash
# number of threads:
for t in 1 
do
	echo NUMT = $t

		g++ Project7.cpp -o prog -lm -fopenmp
		./prog

done
