#!/bin/sh
#SBATCH --partition medium --nodes 128 --time 30 
srun --ntasks 8192 --overcommit -o ranks_8192.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
