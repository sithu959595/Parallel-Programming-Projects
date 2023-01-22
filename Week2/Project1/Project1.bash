#!/bin/bash
# number of threads:
for t in 1 2 4 6 8
do
	echo NUMT = $t

		g++ -DNUMT=$t Project1.cpp -o prog -lm -fopenmp
		./prog

done
