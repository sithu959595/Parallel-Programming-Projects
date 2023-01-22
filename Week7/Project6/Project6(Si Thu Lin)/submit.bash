#!/bin/bash
#SBATCH -J Monyecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o first.out
#SBATCH -e first.err
#SBATCH --mail-type=BEGIN,END,FAIL
#./first
for s in 8 16 32 64 128 256
do

for t in 1 16 64 256 512 1024 2048 4096 8192
do
g++ -DNMB=$t -DLOCAL_SIZE=$s -o first first.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp 
./first
done
done
