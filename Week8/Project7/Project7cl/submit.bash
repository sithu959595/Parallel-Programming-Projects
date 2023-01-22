#!/bin/bash
#SBATCH -J Monyecarlo
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --gres=gpu:1
#SBATCH -o Project7cl.out
#SBATCH -e Project7cl.err
#SBATCH --mail-type=BEGIN,END,FAIL
#./Project7cl
for s in 8
do

for t in 1
do
g++ -o Project7cl Project7cl.cpp /usr/local/apps/cuda/cuda-10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp
./Project7cl
done
done
