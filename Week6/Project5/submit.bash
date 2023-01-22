#!/bin/bash
#SBATCH -J Monyecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o montecarlo.out
#SBATCH -e montecarlo.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=linsi@oregonstate.edu
#./montecarlo
for s in 16 32 64 128
do

for t in 1024 2048 4096 8192 16384 32768 65536 131072 262144 524288 1048576 2097152 4194304 8388608 16777216
do

	/usr/local/apps/cuda/cuda-10.1/bin/nvcc -DBLOCKSIZE=$s -DNUMTRIALS=$t -o montecarlo montecarlo.cu 
	./montecarlo
done
done
