for t in 1
do
	echo NUMT = $t

		g++ -DNUMT=$t Project3.cpp -o prog -lm -fopenmp
		./prog

done
