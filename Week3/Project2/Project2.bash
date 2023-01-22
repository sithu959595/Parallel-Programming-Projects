for t in 1 2 4 6 8 10 12
do
	echo NUMT = $t

		g++ -DNUMT=$t Project2n.cpp -o prog -lm -fopenmp
		./prog

done
