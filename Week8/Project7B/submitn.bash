#!/bin/bash
#SBATCH -J Project7
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o Project7cln.out
#SBATCH -e Project7cln.err
#SBATCH --mail-type=BEGIN,END,FAIL
#./Project7cln
for s in 8
do

for t in 1
do
g++ -o Project7cln Project7cln.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
./Project7cln
done
done
