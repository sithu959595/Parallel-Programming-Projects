for t in 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048578 2097152 4194304  8388608 
do
	echo ARRAYS = $t

	g++ -DARRAYS=$t Project4.cpp -o prog -lm -fopenmp
		./prog

done
