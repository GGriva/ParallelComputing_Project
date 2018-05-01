#!/bin/sh
#SBATCH --partition small --nodes 32 --time 30 
srun --ntasks 2048 --overcommit -o ranks_2048.log ./pp --synch=3 --end=128.0 --batch=4 --gvt-interval=256
