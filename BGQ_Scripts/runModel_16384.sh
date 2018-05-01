#!/bin/sh
#SBATCH --partition medium --nodes 256 --time 5 
srun --ntasks 16384 --overcommit -o ranks_16384.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
