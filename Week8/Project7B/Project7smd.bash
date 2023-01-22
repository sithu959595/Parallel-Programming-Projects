#!/bin/bash
# number of threads:
for t in 1 
do

		g++ Project7smd.cpp -o prog -lm -fopenmp
		./prog

done
