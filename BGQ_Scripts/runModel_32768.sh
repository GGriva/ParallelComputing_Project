#!/bin/sh
#SBATCH --partition medium --nodes 512 --time 5 
srun --ntasks 32768 --overcommit -o ranks_32768.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
